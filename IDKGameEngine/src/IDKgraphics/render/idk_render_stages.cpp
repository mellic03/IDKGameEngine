#include "idk_renderengine.hpp"



void idk::RenderEngine::RenderStage_deferred_geometry( idk::Camera &camera, float dtime )
{
    m_deferred_geom_buffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_deferred_geom_buffer.bind();

    // Non animated models
    // -----------------------------------------------------------------------------------------
    glShader &program = getProgram("geometrypass");
    program.bind();

    for (auto &[model_id, dummy, model_mat]: m_render_queue)
    {
        drawmethods::draw_textured(
            program,
            modelSystem().getModel(model_id),
            model_mat,
            modelSystem().getMaterials()
        );
    }

    m_render_queue.clear();
    // -----------------------------------------------------------------------------------------


    // Animated models
    // -----------------------------------------------------------------------------------------
    glShader &program_anim = getProgram("geometrypass-anim");
    program_anim.bind();

    for (auto &[model_id, animator_id, model_mat]: m_anim_render_queue)
    {
        drawmethods::draw_animated(
            dtime,
            m_UBO_armature,
            animator_id,
            model_id,
            program_anim,
            model_mat,
            modelSystem()
        );
    }

    m_anim_render_queue.clear();
    // -----------------------------------------------------------------------------------------

    for (idk::RenderQueue &rq: m_render_queues)
    {
        const auto &config = rq.config();
        if (config.cull_face == false)
        {
            gl::disable(GL_CULL_FACE);
        }

        glShader &rq_program = getProgram(rq.name());
        rq_program.bind();

        for (auto &[model_id, animator_id, transform]: rq)
        {
            idk::Model &model = modelSystem().getModel(model_id);

            if (model.render_flags & ModelRenderFlag::CHUNKED)
            {
                drawmethods::draw_instanced(
                    rq_program,
                    model_id,
                    transform,
                    modelSystem()
                );
            }

            else
            {
                drawmethods::draw_textured(
                    rq_program,
                    modelSystem().getModel(model_id),
                    transform,
                    modelSystem().getMaterials()
                );
            }
        }

        if (config.cull_face == false)
        {
            gl::enable(GL_CULL_FACE);
        }

        rq.clear();
    }
}


void idk::RenderEngine::RenderStage_deferred_lighting( idk::Camera &camera, float dtime )
{
    gl::enable(GL_BLEND);
    IDK_GLCALL( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); )

    m_scratchbufs1[0].bind();
    m_scratchbufs1[0].clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Background
    // -----------------------------------------------------------------------------------------
    glm::mat4 modelmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.9f * camera.farPlane()));
    modelmat = glm::scale(modelmat, glm::vec3(500.0f, 500.0f, 1.0f));

    glShader &background = getProgram("background");
    background.bind();
    background.set_mat4("un_model", modelmat);
    background.set_samplerCube("un_skybox", skyboxes[current_skybox]);

    gl::bindVertexArray(m_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);
    gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);

    glShader::unbind();
    // -----------------------------------------------------------------------------------------


    // Lighting pass
    // -----------------------------------------------------------------------------------------
    glShader &lighting = getProgram("lighting_ibl");
    lighting.bind();
    lighting.set_sampler2D("un_BRDF_LUT", BRDF_LUT);

    idk::glDepthCascade depthcascade = m_lightsystem.depthCascade();

    lighting.set_vec4("un_cascade_depths", depthcascade.getCascadeDepths(camera.farPlane()));
    lighting.set_sampler2DArray("un_dirlight_depthmap", depthcascade.getTextureArray());

    lighting.set_samplerCube("un_skybox_diffuse",  skyboxes_IBL[current_skybox].first);
    lighting.set_samplerCube("un_skybox_specular", skyboxes_IBL[current_skybox].second);

    tex2tex(lighting, m_deferred_geom_buffer, m_scratchbufs1[0]);

    lighting.popTextureUnits();
    glShader::unbind();
    // -----------------------------------------------------------------------------------------

    gl::disable(GL_BLEND);
}



void
idk::RenderEngine::PostProcess_bloom()
{
    glShader bloom    = getProgram("bloom");
    glShader additive = getProgram("additive");


    bloom.bind();
    tex2tex(bloom, m_scratchbufs1[0], m_scratchbufs2[0]);

    // glShader &downsample = getProgram("downsample");
    // glShader &upsample   = getProgram("upsample");

    // constexpr int miplevel = 7;

    // downsample.bind();
    // tex2tex(downsample, m_scratchbufs2[0], m_scratchbufs1[1]);
    // for (int i=1; i<miplevel; i++)
    // {
    //     tex2tex(downsample, m_scratchbufs1[i], m_scratchbufs1[i+1]);
    // }

    // upsample.bind();
    // tex2tex(upsample, m_scratchbufs1[miplevel], m_scratchbufs1[miplevel], m_scratchbufs2[miplevel-1]);
    // for (int i=miplevel-1; i>0; i--)
    // {
    //     tex2tex(upsample, m_scratchbufs1[i], m_scratchbufs2[i], m_scratchbufs2[i-1]);
    // }



    additive.bind();
    additive.set_float("intensity", getCamera().m_bloom_gamma.x);
    tex2tex(additive, m_scratchbufs1[0], m_scratchbufs2[0], m_mainbuffer_0);


    glShader::unbind();
}




void
idk::RenderEngine::PostProcess_chromatic_aberration( glFramebuffer *buffer_A,
                                                     glFramebuffer *buffer_B )
{
    glShader &chromatic = getProgram("chromatic");
    chromatic.bind();
    tex2tex(chromatic, *buffer_A, *buffer_B);
    glShader::unbind();
}


void
idk::RenderEngine::PostProcess_colorgrading( idk::Camera &camera,
                                             glFramebuffer *buffer_A,
                                             glFramebuffer *buffer_B )
{
    glShader &colorgrade = getProgram("colorgrade");
    colorgrade.bind();

    const GLint secondmip = 4;
    const GLint level  = log2(GL_MAX_TEXTURE_SIZE);
    const GLint level2 = level - secondmip;

    const size_t size   = 4;
    const size_t size2  = size * pow(4, secondmip);
    static float *data  = new float[size];
    static float *data2 = new float[size2];

    IDK_GLCALL( glGetTextureImage(buffer_B->attachments[0], level,  GL_RGBA, GL_FLOAT, size*sizeof(float),  data); )
    IDK_GLCALL( glGetTextureImage(buffer_B->attachments[0], level2, GL_RGBA, GL_FLOAT, size2*sizeof(float), data2); )
 
    float   texw     = m_resolution.x;
    float   texh     = m_resolution.y;

    float   mipw     = glm::max(1.0f, texw / float(pow(2.0f, level2)));
    float   miph     = glm::max(1.0f, texh / float(pow(2.0f, level2)));

    size_t  center_x = size_t(mipw / 2.0f);
    size_t  center_y = size_t(miph / 2.0f);

    size_t idx0 = 4 * (mipw*(center_y-1) + center_x-1);
    size_t idx1 = 4 * (mipw*(center_y-1) + center_x  );
    size_t idx2 = 4 * (mipw*(center_y  ) + center_x-1);
    size_t idx3 = 4 * (mipw*(center_y  ) + center_x  );

    // printf("%.0f  %.0f  %.0f  %.0f  %u  %u\n", texw, texh, mipw, miph, center_x, center_y);
    
    glm::vec3   avg_color  = glm::vec3(data2[idx0+0], data2[idx0+1], data2[idx0+2]);
                avg_color += glm::vec3(data2[idx1+0], data2[idx1+1], data2[idx1+2]);
                avg_color += glm::vec3(data2[idx2+0], data2[idx2+1], data2[idx2+2]);
                avg_color += glm::vec3(data2[idx3+0], data2[idx3+1], data2[idx3+2]);
                avg_color /= 4.0f;

    float alpha = 0.15f;
    avg_color = alpha*avg_color + (1.0f - alpha)*glm::vec3(data[0], data[1], data[2]);

    auto aces = [](glm::vec3 x)
    {
        const float a = 2.51;
        const float b = 0.03;
        const float c = 2.43;
        const float d = 0.59;
        const float e = 0.14;
        glm::vec3 color = glm::clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0f, 1.0f);
        return color;
    };

    avg_color = aces(avg_color);
    float avg_luminance = glm::dot(avg_color, glm::vec3(0.2126, 0.7152, 0.0722));

    auto exposure_curve = [](float luminance)
    {
        float a = -5.0f;
        float b = -0.1f;
        float c = -0.4f;
        float h =  0.9f;
        float v =  0.8f;
        float x = luminance;
    
        float l5 = pow(x-h, 5.0f);
        float l2 = pow(x-h, 2.0f);
        float l1 = x-h;

        return (a*l5 + b*l2 + c*l1 + v);
    };

    float new_exposure  = exposure_curve(avg_luminance);

    float stepsize   = 1.0f / camera.m_bloom_gamma.z;
    float difference = new_exposure - camera.m_exposure.x;
    float direction  = difference / fabs(difference);

    if (fabs(difference) > stepsize)
    {
        camera.m_exposure.x += stepsize*direction;
    }

    tex2tex(colorgrade, *buffer_B, *buffer_A);
    glShader::unbind();
}



void
idk::RenderEngine::RenderStage_postprocessing( idk::Camera &camera )
{
    // Combine geometry and volumetrics
    // -----------------------------------------------------------------------------------------
    glShader &additive = getProgram("additive");
    // additive.bind();
    // additive.set_float("intensity", 1.0f);
    // tex2tex(additive, m_scratchbufs0[0], m_scratchbufs0[2], m_scratchbufs1[0]);
    // glShader::unbind();
    // -----------------------------------------------------------------------------------------

    PostProcess_bloom();

    glFramebuffer *buffer_A = &m_mainbuffer_0;
    glFramebuffer *buffer_B = &m_mainbuffer_1;

    PostProcess_chromatic_aberration(buffer_A, buffer_B);

    buffer_B->generateMipmap(0);

    PostProcess_colorgrading(camera, buffer_A, buffer_B);


    // FXAA
    // -----------------------------------------------------------------------------------------
    glShader &fxaa = getProgram("fxaa");
    fxaa.bind();
    f_fbfb(fxaa, *buffer_A);
    // -----------------------------------------------------------------------------------------



}
