#include "idk_renderengine.h"

#include <filesystem>


GLuint worley_texture;


// Static members -----------------------------
GLuint  idk::RenderEngine::SPHERE_PRIMITIVE;
GLuint  idk::RenderEngine::CUBE_PRIMITIVE;
GLuint  idk::RenderEngine::CRATE_PRIMITIVE;
// --------------------------------------------

void
idk::RenderEngine::f_init_SDL_OpenGL( std::string windowname, size_t w, size_t h)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error creating window\n";
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_SDL_window = SDL_CreateWindow(
        windowname.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);


    m_SDL_gl_context = SDL_GL_CreateContext(m_SDL_window);
    SDL_GL_MakeCurrent(m_SDL_window, m_SDL_gl_context);
    SDL_GL_SetSwapInterval(0); // vsync
    SDL_SetRelativeMouseMode(SDL_FALSE);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error initializing glew\n";
        exit(1);
    }

    gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
}


void
idk::RenderEngine::f_init_screenquad()
{
    float quad_vertices[] = {
      -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
      -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,
       1.0f, -1.0f,  0.0f,  1.0f,  0.0f,

      -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
       1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
       1.0f,  1.0f,  0.0f,  1.0f,  1.0f
    };

    // Send screen quad to GPU
    // ------------------------------------------------------------------------------------
    gl::genVertexArrays(1, &m_quad_VAO);
    gl::genBuffers(1, &m_quad_VBO);

    gl::bindVertexArray(m_quad_VAO);
    gl::bindBuffer(GL_ARRAY_BUFFER, m_quad_VBO);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    gl::vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    gl::enableVertexAttribArray(0);

    gl::vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 3*sizeof(float));
    gl::enableVertexAttribArray(1);
    // ------------------------------------------------------------------------------------
}


void
idk::RenderEngine::compileShaders()
{
    std::cout << "compiling shaders\n";

    m_background_shader = gltools::compileProgram(
        "shaders/deferred/", "background.vs", "background.fs"
    );
    m_deferred_geometrypass_shader = gltools::compileProgram(
        "shaders/deferred/", "geometrypass.vs", "geometrypass.fs"
    );
    m_deferred_lightingpass_shader = gltools::compileProgram(
        "shaders/", "screenquad.vs", "deferred/lightingpass.fs"
    );
    m_dirlight_vol_shader = gltools::compileProgram(
        "shaders/", "screenquad.vs", "deferred/volumetric_dirlight.fs"
    );
    m_guassian_blur_shader = gltools::compileProgram(
        "shaders/", "screenquad.vs", "postprocess/guassianblur.fs"
    );
    m_odd_blur_shader = gltools::compileProgram(
        "shaders/", "screenquad.vs", "postprocess/oddblur.fs"
    );
    m_caberr_shader = gltools::compileProgram(
        "shaders/", "screenquad.vs", "postprocess/c-abberation.fs"
    );
    m_upscale_shader = gltools::compileProgram(
        "shaders/", "screenquad.vs", "postprocess/upsample.fs"
    );
    m_downscale_shader = gltools::compileProgram(
        "shaders/", "screenquad.vs", "postprocess/downsample.fs"
    );
    m_SSR_shader = gltools::compileProgram(
        "shaders/", "screenquad.vs", "postprocess/SSR.fs"
    );
    m_blank_shader = gltools::compileProgram(
        "shaders/", "screenquad.vs", "postprocess/blank.fs"
    );
    m_blit_shader = gltools::compileProgram(
        "shaders/", "screenquad.vs", "postprocess/blit.fs"
    );

    m_additive_shader   = gltools::compileProgram("shaders/", "screenquad.vs", "postprocess/additive.fs");
    m_colorgrade_shader = gltools::compileProgram("shaders/", "screenquad.vs", "postprocess/colorgrade.fs");
    m_fxaa_shader       = gltools::compileProgram("shaders/", "screenquad.vs", "postprocess/fxaa.fs");
    m_dirshadow_shader  = gltools::compileProgram("shaders/", "dirshadow.vs", "dirshadow.fs");
    solid_shader        = gltools::compileProgram("shaders/", "vsin_pos_only.vs", "solid.fs");
}


void
idk::RenderEngine::f_gen_idk_framebuffers( int w, int h )
{
    idk::ColorAttachmentConfig config = {
        .internalformat = GL_RGBA16F,
        .minfilter      = GL_LINEAR,
        .magfilter      = GL_LINEAR,
        .datatype       = GL_FLOAT
    };

    idk::DepthAttachmentConfig depth_config = {
        .internalformat = GL_DEPTH_COMPONENT16,
        .datatype       = GL_FLOAT
    };


    m_scratchbufs0.resize(NUM_SCRATCH_BUFFERS);
    m_scratchbufs1.resize(NUM_SCRATCH_BUFFERS);
    m_scratchbufs2.resize(NUM_SCRATCH_BUFFERS);
    m_scratchbufs3.resize(NUM_SCRATCH_BUFFERS);

    for (size_t i=0; i<NUM_SCRATCH_BUFFERS; i++)
    {
        int width  = w / pow(2, i);
        int height = h / pow(2, i);

        m_scratchbufs0[i].reset(width, height, ATTACHMENTS_PER_BUFFER);
        m_scratchbufs1[i].reset(width, height, ATTACHMENTS_PER_BUFFER);
        m_scratchbufs2[i].reset(width, height, ATTACHMENTS_PER_BUFFER);
        m_scratchbufs3[i].reset(width, height, ATTACHMENTS_PER_BUFFER);

        for (size_t j=0; j<ATTACHMENTS_PER_BUFFER; j++)
        {
            m_scratchbufs0[i].colorAttachment(j, config);
            m_scratchbufs1[i].colorAttachment(j, config);
            m_scratchbufs2[i].colorAttachment(j, config);
            m_scratchbufs3[i].colorAttachment(j, config);
        }
    }

    m_deferred_geom_buffer.reset(w, h, 4);
    m_deferred_geom_buffer.colorAttachment(0, config);
    m_deferred_geom_buffer.colorAttachment(1, config);
    m_deferred_geom_buffer.colorAttachment(2, config);
    m_deferred_geom_buffer.colorAttachment(3, config);

    m_volumetrics_buffer.reset(w/4, h/4, 1);
    m_volumetrics_buffer.colorAttachment(0, config);

    m_dirlight_depthmap_buffer.reset(4096, 4096, 1);
    m_dirlight_depthmap_buffer.colorAttachment(0, config);

    m_mainbuffer_0.reset(w, h, 1);
    m_mainbuffer_0.colorAttachment(0, config);
    m_mainbuffer_0.depthAttachment(depth_config);

    m_mainbuffer_1.reset(w, h, 1);
    m_mainbuffer_1.colorAttachment(0, config);
    m_mainbuffer_1.depthAttachment(depth_config);
}



idk::RenderEngine::RenderEngine( std::string name, int w, int h, int res_divisor )
{
    gltools::init();

    m_resolution = glm::ivec2(w, h);
    f_init_SDL_OpenGL(name, w, h);
    f_init_screenquad();

    RenderEngine::SPHERE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::icosphere, "");
    RenderEngine::CUBE_PRIMITIVE   = modelManager().loadOBJ(idk::objprimitives::cube, "");
    RenderEngine::CRATE_PRIMITIVE  = modelManager().loadOBJ(idk::objprimitives::crate, "");
   
    f_gen_idk_framebuffers(w, h);
    compileShaders();


    m_active_camera_id = createCamera();

    m_UBO_camera      = glUBO(2, 2*sizeof(glm::mat4) + sizeof(glm::vec4));
    m_UBO_pointlights = glUBO(3, 16 + IDK_MAX_POINTLIGHTS*sizeof(Pointlight));
    m_UBO_spotlights  = glUBO(4, 16 + IDK_MAX_SPOTLIGHTS*sizeof(Spotlight));
    m_UBO_dirlights   = glUBO(5, 16 + IDK_MAX_DIRLIGHTS*sizeof(Dirlight) + IDK_MAX_DIRLIGHTS*sizeof(glm::mat4));

    worley_texture = noisegen3D::worley();
}




void
idk::RenderEngine::f_fbfb( GLuint shader, glFramebuffer &in )
{
    in.unbind();
    gltools::useProgram(shader);

    for (size_t i=0; i < in.attachments.size(); i++)
        gltools::setUniform_texture("un_texture_" + std::to_string(i), in.attachments[0]);

    gl::bindVertexArray(m_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    gltools::freeTextureUnitIDs();
}


void
idk::RenderEngine::tex2tex( GLuint program, glFramebuffer &in, glFramebuffer &out )
{
    out.bind();
    gltools::useProgram(program);

    static float inc0 = 0.0f;
    static float inc1 = 0.0f;
    static float inc2 = 0.0f;
    gltools::setUniform_texture("un_dirlight_depthmaps[0]", m_dirlight_shadowmap_allocator.get(0));
    gltools::setUniform_texture3D("un_worley", worley_texture);
    gltools::setUniform_float("un_increment_0", inc0);
    gltools::setUniform_float("un_increment_1", inc1);
    gltools::setUniform_float("un_increment_2", inc2);

    gltools::setUniform_vec2("un_r_offset", m_abbr_str*m_r_abbr);
    gltools::setUniform_vec2("un_g_offset", m_abbr_str*m_g_abbr);
    gltools::setUniform_vec2("un_b_offset", m_abbr_str*m_b_abbr);

    inc0 += 0.0001f;
    inc1 += 0.000015f;
    inc2 += 0.00003f;


    for (size_t i=0; i < in.attachments.size(); i++)
    {
        gltools::setUniform_texture(
            "un_texture_" + std::to_string(i),
            in.attachments[i]
        );
    }
    if (in.has_depth)
        gltools::setUniform_texture("un_depth", in.depth_attachment);

    gl::drawArrays(GL_TRIANGLES, 0, 6);

    gltools::freeTextureUnitIDs();
};



/** Run a shader with two input framebuffers, `a` and `b`.
 *  Render the result to `out`.
 * 
 *  @note `a`, `b` and `out` cannot have more than four textures each.
 *  @note the resolution of `out` determines the render resolution.
 * 
*/
void
idk::RenderEngine::tex2tex( GLuint program, glFramebuffer &a, glFramebuffer &b, glFramebuffer &out )
{
    out.bind();
    gltools::useProgram(program);


    static float inc0 = 0.0f;
    static float inc1 = 0.0f;
    static float inc2 = 0.0f;
    gltools::setUniform_texture("un_dirlight_depthmaps[0]", m_dirlight_shadowmap_allocator.get(0));
    gltools::setUniform_texture3D("un_worley", worley_texture);
    gltools::setUniform_float("un_increment_0", inc0);
    gltools::setUniform_float("un_increment_1", inc1);
    gltools::setUniform_float("un_increment_2", inc2);


    inc0 += 0.0001f;
    inc1 += 0.000015f;
    inc2 += 0.00003f;


    size_t textureID = 0;
    for (size_t i=0; i < a.attachments.size(); i++)
    {
        gltools::setUniform_texture(
            "un_texture_" + std::to_string(textureID),
            a.attachments[i]
        );

        textureID += 1;
    }
    if (a.has_depth)
        gltools::setUniform_texture("un_depth_a", a.depth_attachment);



    textureID = 4;
    for (size_t i=0; i < b.attachments.size(); i++)
    {
        gltools::setUniform_texture(
            "un_texture_" + std::to_string(textureID),
            b.attachments[i]
        );

        textureID += 1;
    }
    if (b.has_depth)
        gltools::setUniform_texture("un_depth_b", b.depth_attachment);

    gl::drawArrays(GL_TRIANGLES, 0, 6);

    gltools::freeTextureUnitIDs();
};





int
idk::RenderEngine::createCamera()
{
    int camera_id = m_camera_allocator.create();
    m_camera_allocator.get(camera_id).aspect(m_resolution.x, m_resolution.y);
    return camera_id;
}


int
idk::RenderEngine::createPointlight()
{
    return m_pointlight_allocator.create();
}


int
idk::RenderEngine::createSpotlight()
{
    return m_spotlight_allocator.create();
}


int
idk::RenderEngine::createDirlight()
{
    m_dirlight_shadowmap_allocator.create();
    m_dirlight_lightspacematrix_allocator.create();
    return m_dirlight_allocator.create();
}


void
idk::RenderEngine::drawModel( GLuint shader_id, int model_id, Transform &transform )
{
    m_model_draw_queue[shader_id].push({model_id, transform});
}

void
idk::RenderEngine::drawWireframe( GLuint shader_id, int model_id, Transform &transform )
{
    m_wireframe_draw_queue[shader_id].push({model_id, transform});
}




#define ORTHO_N 0.01f
#define ORTHO_F 150.0f
#define ORTHO_W 50.0f
#define ORTHO_R 0.2f

void
idk::RenderEngine::f_shadowpass_dirlights()
{
    m_dirlight_depthmap_buffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gltools::useProgram(m_dirshadow_shader);


    RenderEngine &ren = *this;
    auto &queue = m_model_draw_queue;
    auto &depthmaps = m_dirlight_shadowmap_allocator;
    auto &framebuffer = m_dirlight_depthmap_buffer;
    auto quadVAO = m_quad_VAO;

    int i = 0;
    dirlights().for_each(
    [&i, &ren, &queue, &depthmaps, &framebuffer, quadVAO](int light_id, Dirlight &light)
    {
        glm::mat4 projection = glm::ortho(-ORTHO_W, ORTHO_W, -ORTHO_W, ORTHO_W, ORTHO_N, ORTHO_F);
        glm::mat4 view = glm::lookAt(
            ren.getCamera().transform().position() - (ORTHO_R*ORTHO_F)*glm::normalize(glm::vec3(light.direction)),
            glm::vec3(0.0f) + ren.getCamera().transform().position(),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        glm::mat4 lightspacematrix = projection * view;
        gltools::setUniform_mat4("un_lightspacematrix", lightspacematrix);


        glm::mat4 modelmat = glm::scale(glm::mat4(1.0f), glm::vec3(300.0f, 300.0f, 1.0f));
        modelmat = glm::translate(modelmat, glm::vec3(0.0f, 0.0f, -0.9*ORTHO_F));
        gltools::setUniform_mat4("un_model", glm::inverse(view) * modelmat);
        gl::bindVertexArray(quadVAO);
        gl::drawArrays(GL_TRIANGLES, 0, 6);
        gl::bindVertexArray(0);


        for (auto &[shader_id, vec]: queue)
        {
            for (auto &[model_id, transform]: vec)
            {
                drawmethods::draw_untextured(
                    ren.modelManager().getModel(model_id),
                    transform
                );
            }
        }

        depthmaps.get(light_id) = framebuffer.attachments[0];
        i += 1;
    });
}


void
idk::RenderEngine::f_update_UBO_camera()
{
    idk::Camera &camera = getCamera();
    m_UBO_camera.bind();
    m_UBO_camera.add<glm::mat4>(glm::value_ptr(camera.view()));
    m_UBO_camera.add<glm::mat4>(glm::value_ptr(camera.projection()));
    m_UBO_camera.add<glm::vec3>(glm::value_ptr(camera.transform().position()));
    m_UBO_camera.unbind();
}


void
idk::RenderEngine::f_update_UBO_pointlights()
{
    int num_pointlights = pointlights().size();

    std::vector<idk::Pointlight> lights;
    pointlights().for_each(
        [&lights](Pointlight &light)
        {
            lights.push_back(light);
        }
    );
    lights.resize(IDK_MAX_POINTLIGHTS);

    m_UBO_pointlights.bind();
    m_UBO_pointlights.add<int>(&num_pointlights);
    m_UBO_pointlights.add(sizeof(Pointlight)*IDK_MAX_SPOTLIGHTS, &(lights[0]));
    m_UBO_pointlights.unbind();
}


void
idk::RenderEngine::f_update_UBO_spotlights()
{
    int num_spotlights = spotlights().size();

    idk::vector<idk::Spotlight> lights;
    spotlights().for_each(
        [&lights](Spotlight &light)
        {
            lights.push(light);
        }
    );
    lights.resize(IDK_MAX_SPOTLIGHTS);

    m_UBO_spotlights.bind();
    m_UBO_spotlights.add<int>(&num_spotlights);
    m_UBO_spotlights.add(sizeof(Spotlight)*IDK_MAX_SPOTLIGHTS, lights.data());
    m_UBO_spotlights.unbind();
}


void
idk::RenderEngine::f_update_UBO_dirlights()
{
    int num_dirlights = dirlights().size();

    idk::vector<idk::Dirlight> lights;
    idk::vector<glm::mat4> matrices;

    idk::Camera &camera = getCamera();

    dirlights().for_each(
        [&lights, &matrices, &camera](Dirlight &light)
        {
            lights.push(light);

            glm::mat4 projection = glm::ortho(-ORTHO_W, ORTHO_W, -ORTHO_W, ORTHO_W, ORTHO_N, ORTHO_F);
            glm::mat4 view = glm::lookAt(
                camera.transform().position() - (ORTHO_R*ORTHO_F)*glm::normalize(glm::vec3(light.direction)),
                glm::vec3(0.0f) + camera.transform().position(),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            glm::mat4 lightspacematrix = projection * view;
            matrices.push(lightspacematrix);
        }
    );


    lights.resize(IDK_MAX_DIRLIGHTS);
    matrices.resize(IDK_MAX_DIRLIGHTS);

    m_UBO_dirlights.bind();
    m_UBO_dirlights.add<int>(&num_dirlights);
    m_UBO_dirlights.add(sizeof(Dirlight)*IDK_MAX_DIRLIGHTS, lights.data());
    m_UBO_dirlights.add(sizeof(glm::mat4)*IDK_MAX_DIRLIGHTS, matrices.data());
    m_UBO_dirlights.unbind();
}


void
idk::RenderEngine::beginFrame()
{
    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);
    gl::viewport(0, 0, m_resolution.x, m_resolution.y);
    gl::clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void
idk::RenderEngine::endFrame()
{
    gl::disable(GL_CULL_FACE);
    f_shadowpass_dirlights();
    gl::enable(GL_CULL_FACE);

    f_update_UBO_camera();
    f_update_UBO_pointlights();
    f_update_UBO_spotlights();
    f_update_UBO_dirlights();

    idk::Camera &camera = getCamera();

    // Deferred geometry pass --------------------------------------------
    m_deferred_geom_buffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_deferred_geom_buffer.bind();
    for (auto &[shader_id, vec]: m_model_draw_queue)
    {
        gltools::useProgram(shader_id);

        for (auto &[model_id, transform]: vec)
        {
            drawmethods::draw_textured(
                modelManager().getModel(model_id),
                transform,
                modelManager().getMaterials()
            );
            gltools::freeTextureUnitIDs();
        }
        vec.clear();
    }
    m_model_draw_queue.clear();
    gl::bindVertexArray(0);
    // -------------------------------------------------------------------


    // Wireframe objects -------------------------------------------------
    // gl::disable(GL_CULL_FACE);
    // gltools::useProgram(solid_shader);
    // gltools::setUniform_vec3("un_color", glm::vec3(1.0f));
    // for (auto &[shader_id, vec]: m_wireframe_draw_queue)
    // {
    //     gltools::useProgram(shader_id);

    //     for (auto &[model_id, transform]: vec)
    //     {
    //         drawmethods::draw_wireframe(
    //             modelManager().getModel(model_id),
    //             transform
    //         );
    //         gltools::freeTextureUnitIDs();
    //     }
    //     vec.clear();
    // }
    // m_wireframe_draw_queue.clear();
    // gl::enable(GL_CULL_FACE);
    // gl::bindVertexArray(0);
    // -----------------------------------------------------------------------------------------


    // Background quad
    // -----------------------------------------------------------------------------------------
    gl::bindVertexArray(m_quad_VAO);
    gltools::useProgram(m_background_shader);
    glm::mat4 modelmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f * camera.farPlane()));
    modelmat = glm::scale(modelmat, glm::vec3(300.0f, 300.0f, 1.0f));
    gltools::setUniform_mat4("un_model", modelmat);
    gl::drawArrays(GL_TRIANGLES, 0, 6);
    gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);
    // -----------------------------------------------------------------------------------------


    // Blinn-Phong lighting
    // -----------------------------------------------------------------------------------------
    tex2tex(m_deferred_lightingpass_shader, m_deferred_geom_buffer, m_mainbuffer_1);
    // -----------------------------------------------------------------------------------------


    // Volumetric directional lights
    // -----------------------------------------------------------------------------------------
    tex2tex(m_dirlight_vol_shader, m_deferred_geom_buffer,   m_scratchbufs0[1]);
    tex2tex(m_upscale_shader,      m_scratchbufs0[1],        m_scratchbufs1[0]);
    // -----------------------------------------------------------------------------------------


    // Combine geometry and volumetrics
    // -----------------------------------------------------------------------------------------
    gltools::useProgram(m_additive_shader);
    gltools::setUniform_float("intensity", 1.0f);
    tex2tex(m_additive_shader, m_mainbuffer_1, m_scratchbufs1[0], m_mainbuffer_0);
    // -----------------------------------------------------------------------------------------

    // SSR
    // -----------------------------------------------------------------------------------------
    // tex2tex(m_SSR_shader, m_deferred_geom_buffer, m_scratchbuf1, m_scratchbuf0_d2);
    // gltools::useProgram(m_additive_shader);
    // gltools::setUniform_float("intensity", 1.0f);
    // tex2tex(m_additive_shader, m_scratchbuf1, m_scratchbuf3, m_scratchbuf2);
    // -----------------------------------------------------------------------------------------


    // Blit user framebuffers
    // -----------------------------------------------------------------------------------------
    while (m_blit_queue.empty() == false)
    {
        idk::glFramebuffer &framebuffer = m_blit_queue.front();
        tex2tex(m_blit_shader, framebuffer, m_mainbuffer_0, m_mainbuffer_1);
        m_blit_queue.pop();
    }
    // -----------------------------------------------------------------------------------------


    // Chromatic aberration
    // -----------------------------------------------------------------------------------------
    tex2tex(m_caberr_shader, m_mainbuffer_1, m_mainbuffer_0);
    // -----------------------------------------------------------------------------------------


    // Color grading
    // -----------------------------------------------------------------------------------------
    gltools::useProgram(m_colorgrade_shader);
    gltools::setUniform_float("un_gamma", m_gamma);
    gltools::setUniform_float("un_exposure", m_exposure);
    tex2tex(m_colorgrade_shader, m_mainbuffer_0, m_mainbuffer_1);
    // -----------------------------------------------------------------------------------------


    // FXAA
    // -----------------------------------------------------------------------------------------
    f_fbfb(m_fxaa_shader, m_mainbuffer_1);
    // -----------------------------------------------------------------------------------------

    gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
    gl::bindVertexArray(0);
}


void
idk::RenderEngine::swapWindow()
{
    SDL_GL_SwapWindow(m_SDL_window);
}


void
idk::RenderEngine::resize( int w, int h )
{
    m_resolution.x = w;  m_resolution.y = h;
    f_gen_idk_framebuffers(w, h);
    getCamera().aspect(w, h);
}



/** Blit an idk::glFramebuffer onto the renderer's main framebuffer including depth.
 *  @note This is a mildly expensive operation, try to avoid.
*/
void
idk::RenderEngine::blitFramebuffer( const idk::glFramebuffer &fb )
{
    m_blit_queue.push(fb);
}


