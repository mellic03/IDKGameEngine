#include "idk_renderengine.h"

#include <filesystem>


GLuint worley_texture;
GLuint whitenoise;


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
    createProgram("background",      "IDKGE/shaders/deferred/", "background.vs", "background.fs");
    createProgram("geometry_pass",   "IDKGE/shaders/deferred/", "geometrypass.vs", "geometrypass.fs");
    createProgram("geometry_light",  "IDKGE/shaders/deferred/", "geometrypass.vs", "lightsource.fs");
    // createProgram("lighting_pass",   "IDKGE/shaders/", "screenquad.vs", "deferred/lightingpass_pbr.fs");

    // glShader &program = getProgram("lighting_pass");
    // program.setDefinition("MAX_POINTLIGHTS", std::to_string(IDK_MAX_POINTLIGHTS));
    // program.setDefinition("MAX_SPOTLIGHTS",  std::to_string(IDK_MAX_SPOTLIGHTS));
    // program.setDefinition("MAX_DIRLIGHTS",   std::to_string(IDK_MAX_DIRLIGHTS));
    // program.compile();

    createProgram("dirvolumetrics", "IDKGE/shaders/", "screenquad.vs", "deferred/volumetric_dirlight.fs");
    createProgram("gaussian",       "IDKGE/shaders/", "screenquad.vs", "postprocess/gaussian.fs");
    createProgram("additive",       "IDKGE/shaders/", "screenquad.vs", "postprocess/additive.fs");
    createProgram("chromatic",      "IDKGE/shaders/", "screenquad.vs", "postprocess/c-abberation.fs");
    createProgram("blit",           "IDKGE/shaders/", "screenquad.vs", "postprocess/blit.fs");
    createProgram("fxaa",           "IDKGE/shaders/", "screenquad.vs", "postprocess/fxaa.fs");
    createProgram("getdepth",       "IDKGE/shaders/", "screenquad.vs", "postprocess/getdepth.fs");
    createProgram("colorgrade",     "IDKGE/shaders/", "screenquad.vs", "postprocess/colorgrade.fs");
    createProgram("dir_shadow",     "IDKGE/shaders/", "dirshadow.vs",  "dirshadow.fs");
    createProgram("solid",          "IDKGE/shaders/", "vsin_pos_only.vs", "solid.fs");
}


void
idk::RenderEngine::f_init_framebuffers( int w, int h )
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

    m_mainbuffer_0.reset(w, h, 2);
    m_mainbuffer_0.colorAttachment(0, config);
    m_mainbuffer_0.colorAttachment(1, config);

    m_mainbuffer_1.reset(w, h, 1);
    m_mainbuffer_1.colorAttachment(0, config);

}



idk::RenderEngine::RenderEngine( std::string name, int w, int h, int res_divisor )
{
    m_resolution = glm::ivec2(w, h);
    f_init_SDL_OpenGL(name, w, h);
    f_init_screenquad();
    f_init_framebuffers(w, h);
    m_lightsystem.init();

    RenderEngine::SPHERE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::icosphere, "");
    RenderEngine::CUBE_PRIMITIVE   = modelManager().loadOBJ(idk::objprimitives::cube, "");
    RenderEngine::CRATE_PRIMITIVE  = modelManager().loadOBJ(idk::objprimitives::crate, "");
   
    compileShaders();

    m_active_camera_id = createCamera();

    m_UBO_camera      = glUBO(2, 2*sizeof(glm::mat4) + sizeof(glm::vec4));
    m_UBO_pointlights = glUBO(3, 16 + IDK_MAX_POINTLIGHTS*sizeof(Pointlight));
    // m_UBO_spotlights  = glUBO(4, 16 + IDK_MAX_SPOTLIGHTS*sizeof(Spotlight));
    m_UBO_dirlights   = glUBO(5, IDK_MAX_DIRLIGHTS * (sizeof(Dirlight) + sizeof(glm::mat4)));

    worley_texture = noisegen3D::worley(64);
    whitenoise     = noisegen3D::white(512, 512, 32);
}


GLuint
idk::RenderEngine::createProgram( std::string name, std::string root, std::string vs, std::string fs )
{
    m_shader_names.push_back(name);
    return m_shaders[name].loadFileC(root, vs, fs);
}


void
idk::RenderEngine::f_fbfb( glShader &program, glFramebuffer &in )
{
    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);

    for (size_t i=0; i < in.attachments.size(); i++)
        program.set_sampler2D("un_texture_" + std::to_string(i), in.attachments[i]);

    gl::drawArrays(GL_TRIANGLES, 0, 6);
}



void
idk::RenderEngine::tex2tex( glShader &program, glFramebuffer &in, glFramebuffer &out )
{
    out.bind();

    static float inc0 = 0.0f;
    static float inc1 = 0.0f;
    static float inc2 = 0.0f;
    program.set_sampler3D("un_worley", worley_texture);
    program.set_sampler3D("un_whitenoise", whitenoise);

    program.set_float("un_increment_0", inc0);
    program.set_float("un_increment_1", inc1);
    program.set_float("un_increment_2", inc2);

    inc0 += 0.0001f;
    inc1 += 0.000015f;
    inc2 += 0.015f;

    for (size_t i=0; i < in.attachments.size(); i++)
    {
        program.set_sampler2D("un_texture_" + std::to_string(i), in.attachments[i]);
    }

    gl::drawArrays(GL_TRIANGLES, 0, 6);
};



void
idk::RenderEngine::tex2tex( glShader &program, glFramebuffer &a, glFramebuffer &b, glFramebuffer &out )
{
    out.bind();

    static float inc0 = 0.0f;
    static float inc1 = 0.0f;
    static float inc2 = 0.0f;
    program.set_sampler3D("un_worley", worley_texture);
    program.set_sampler3D("un_whitenoise", whitenoise);

    program.set_float("un_increment_0", inc0);
    program.set_float("un_increment_1", inc1);
    program.set_float("un_increment_2", inc2);

    inc0 += 0.0001f;
    inc1 += 0.000015f;
    inc2 += 0.015f;
    inc2 += 0.00003f;


    size_t textureID = 0;
    for (size_t i=0; i < a.attachments.size(); i++)
    {
        program.set_sampler2D("un_texture_" + std::to_string(i), a.attachments[i]);
    }

    for (size_t i=0; i < b.attachments.size(); i++)
    {
        program.set_sampler2D("un_texture_" + std::to_string(4+i), b.attachments[i]);
    }

    gl::drawArrays(GL_TRIANGLES, 0, 6);
};



int
idk::RenderEngine::createCamera()
{
    int camera_id = m_camera_allocator.create();
    m_camera_allocator.get(camera_id).aspect(m_resolution.x, m_resolution.y);
    return camera_id;
}



void
idk::RenderEngine::drawModel( GLuint shader_id, int model_id, glm::mat4 &model_mat )
{
    m_model_draw_queue[shader_id].push({model_id, model_mat});
}


void
idk::RenderEngine::drawShadowCaster( int model_id, glm::mat4 &model_mat )
{
    m_shadowcast_queue.push({model_id, model_mat});
}


void
idk::RenderEngine::drawModel_now( glShader &program, int model_id, glm::mat4 &model_mat )
{
    drawmethods::draw_textured(
        program,
        modelManager().getModel(model_id),
        model_mat,
        modelManager().getMaterials()
    );
}


void
idk::RenderEngine::drawPointlight( Pointlight light )
{
    m_pointlight_queue.push(light);
}



#define ORTHO_N 0.01f
#define ORTHO_F 150.0f
#define ORTHO_W 50.0f
#define ORTHO_R 0.2f

void
idk::RenderEngine::f_shadowpass_dirlights()
{
    idk::Camera &cam = getCamera();

    idk::glShader &program = getProgram("dir_shadow");
    program.bind();

    auto &queue  = m_model_draw_queue;
    auto quadVAO = m_quad_VAO;

    glm::mat4 projection = glm::ortho(-ORTHO_W, ORTHO_W, -ORTHO_W, ORTHO_W, ORTHO_N, ORTHO_F);

    idk::vector<idk::Dirlight> &dirlights       = m_lightsystem.dirlights();
    idk::vector<idk::glFramebuffer> &shadowmaps = m_lightsystem.shadowmaps();

    for (int i=0; i<dirlights.size(); i++)
    {
        shadowmaps[i].clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::vec3 dir = glm::normalize(glm::vec3(dirlights[i].direction));
        dirlights[i].direction = glm::vec4(dir, 0.0f);

        glm::mat4 view = glm::lookAt(
            cam.transform().position() - ORTHO_R*ORTHO_F*dir,
            glm::vec3(0.0f) + cam.transform().position(),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        program.set_mat4("un_lightspacematrix", projection * view);


        glm::mat4 modelmat = glm::scale(glm::mat4(1.0f), glm::vec3(300.0f, 300.0f, 1.0f));
        modelmat = glm::translate(modelmat, glm::vec3(0.0f, 0.0f, -0.9*ORTHO_F));
        program.set_mat4("un_model", glm::inverse(view) * modelmat);

        gl::bindVertexArray(quadVAO);
        gl::drawArrays(GL_TRIANGLES, 0, 6);
        gl::bindVertexArray(0);


        for (auto &[model_id, model_mat]: m_shadowcast_queue)
        {
            drawmethods::draw_untextured(
                program,
                modelManager().getModel(model_id),
                model_mat
            );
        }
    
        shadowmaps[i].unbind();
    }


    glShader::unbind();
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
idk::RenderEngine::f_update_UBO_dirlights()
{
    idk::Camera &cam = getCamera();

    idk::vector<Dirlight>  lights(IDK_MAX_DIRLIGHTS);
    idk::vector<glm::mat4> matrices(IDK_MAX_DIRLIGHTS);

    glm::mat4 projection = glm::ortho(-ORTHO_W, ORTHO_W, -ORTHO_W, ORTHO_W, ORTHO_N, ORTHO_F);
    idk::vector<idk::Dirlight> &dirlights = m_lightsystem.dirlights();

    for (int i=0; i<dirlights.size(); i++)
    {
        const glm::vec3 dir = glm::normalize(glm::vec3(dirlights[i].direction));
        dirlights[i].direction = glm::vec4(dir, 0.0f);

        glm::mat4 view = glm::lookAt(
            cam.transform().position() - ORTHO_R*ORTHO_F*dir,
            glm::vec3(0.0f) + cam.transform().position(),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        lights[i]   = dirlights[i];
        matrices[i] = projection * view;
    }


    m_UBO_dirlights.bind();
    m_UBO_dirlights.add(IDK_MAX_DIRLIGHTS * sizeof(Dirlight),  lights.data());
    m_UBO_dirlights.add(IDK_MAX_DIRLIGHTS * sizeof(glm::mat4), matrices.data());
    m_UBO_dirlights.unbind();
}


void
idk::RenderEngine::f_update_UBO_pointlights()
{
    idk::Camera &cam = getCamera();

    idk::vector<Pointlight> &pointlights = m_lightsystem.pointlights();
    idk::vector<Pointlight>  lights(IDK_MAX_POINTLIGHTS);

    for (int i=0; i<pointlights.size(); i++)
    {
        lights[i] = pointlights[i];
    }

    m_UBO_pointlights.bind();
    m_UBO_pointlights.add(IDK_MAX_DIRLIGHTS * sizeof(Pointlight),  lights.data());
    m_UBO_pointlights.unbind();
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
    if (m_lightsystem.changed())
    {
        static std::string vert_source = "";
        static std::string frag_source = "";
        m_lightsystem.genShaderString(vert_source, frag_source);
        // std::cout << "\n\n" << frag_source << "\n\n";

        glShader &lighting = getProgram("lighting_pass");
        lighting.loadStringC(vert_source, frag_source);
    }

    gl::disable(GL_CULL_FACE);
    f_shadowpass_dirlights();
    m_shadowcast_queue.clear();
    gl::enable(GL_CULL_FACE);


    f_update_UBO_camera();
    f_update_UBO_dirlights();
    f_update_UBO_pointlights();


    idk::Camera &camera = getCamera();

    // Deferred geometry pass
    // -----------------------------------------------------------------------------------------
    m_deferred_geom_buffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_deferred_geom_buffer.bind();

    glShader &geometrypass = getProgram("geometry_pass");
    geometrypass.bind();

    for (auto &[shader_id, vec]: m_model_draw_queue)
    {
        for (auto &[model_id, model_mat]: vec)
        {
            drawmethods::draw_textured(
                geometrypass,
                modelManager().getModel(model_id),
                model_mat,
                modelManager().getMaterials()
            );
        }
        vec.clear();
    }
    glShader::unbind();
    m_model_draw_queue.clear();

    getProgram("geometry_light").bind();
    for (Pointlight &light: m_pointlight_queue)
    {
        glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3(light.position));
        mat = glm::scale(mat, glm::vec3(0.1f));
    
        getProgram("geometry_light").set_vec4("un_color", light.diffuse);

        drawmethods::draw_untextured(
            geometrypass,
            modelManager().getModel(SPHERE_PRIMITIVE),
            mat
        );
    }
    m_pointlight_queue.clear();
    glShader::unbind();

    gl::bindVertexArray(0);
    // -----------------------------------------------------------------------------------------
    

    // Background quad
    // -----------------------------------------------------------------------------------------
    glm::mat4 modelmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f * camera.farPlane()));
    modelmat = glm::scale(modelmat, glm::vec3(300.0f, 300.0f, 1.0f));

    gl::bindVertexArray(m_quad_VAO);
    glShader &background = getProgram("background");
    background.bind();
    background.set_mat4("un_model", modelmat);

    gl::drawArrays(GL_TRIANGLES, 0, 6);
    gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);

    glShader::unbind();
    // -----------------------------------------------------------------------------------------


    // Blinn-Phong lighting
    // -----------------------------------------------------------------------------------------
    glShader &lighting = m_shaders["lighting_pass"];
    lighting.bind();

    idk::vector<glFramebuffer> &shadowmaps = m_lightsystem.shadowmaps();

    for (int i=0; i<m_lightsystem.dirlights().size(); i++)
    {
        lighting.set_sampler2D(
            "un_dirlight_depthmaps[" + std::to_string(i) + "]", shadowmaps[i].attachments[0]
        );
    }

    tex2tex(lighting, m_deferred_geom_buffer, m_scratchbufs0[0]);

    lighting.popTextureUnits();
    glShader::unbind();
    // -----------------------------------------------------------------------------------------


    // Volumetric directional lights
    // -----------------------------------------------------------------------------------------
    glShader &dvol = m_shaders["dirvolumetrics"];
    dvol.bind();
    for (int i=0; i<m_lightsystem.dirlights().size(); i++)
    {
        dvol.set_sampler2D(
            "un_dirlight_depthmaps[" + std::to_string(i) + "]", shadowmaps[i].attachments[0]
        );
    }

    tex2tex(dvol, m_deferred_geom_buffer,   m_scratchbufs0[2]);
    glShader::unbind();
    // -----------------------------------------------------------------------------------------


    // Combine geometry and volumetrics
    // -----------------------------------------------------------------------------------------
    glShader &additive = getProgram("additive");
    additive.bind();
    additive.set_float("intensity", 1.0f);
    tex2tex(additive, m_scratchbufs0[0], m_scratchbufs0[2], m_mainbuffer_0);
    glShader::unbind();
    // -----------------------------------------------------------------------------------------


    glFramebuffer *buffer_a = &m_mainbuffer_0;
    glFramebuffer *buffer_b = &m_mainbuffer_1;

    // Extract depth
    // -----------------------------------------------------------------------------------------
    // glShader &getdepth = getProgram("getdepth");
    // getdepth.bind();
    // tex2tex(getdepth, m_deferred_geom_buffer, m_mainbuffer_0);
    // glShader::unbind();
    // -----------------------------------------------------------------------------------------

    // Blit user framebuffers
    // -----------------------------------------------------------------------------------------
    // glShader &blit = getProgram("blit");
    // blit.bind();
    // while (m_blit_queue.empty() == false)
    // {
    //     idk::glFramebuffer &framebuffer = m_blit_queue.front();
    //     tex2tex(blit, framebuffer, *buffer_a, *buffer_b);
    //     m_blit_queue.pop();
    
    //     idk::swap(buffer_a, buffer_b);
    // }
    // glShader::unbind();
    // -----------------------------------------------------------------------------------------


    // Chromatic aberration
    // -----------------------------------------------------------------------------------------
    glShader &chromatic = getProgram("chromatic");
    chromatic.bind();
    chromatic.set_vec2("un_r_offset", m_abbr_str*m_r_abbr);
    chromatic.set_vec2("un_g_offset", m_abbr_str*m_g_abbr);
    chromatic.set_vec2("un_b_offset", m_abbr_str*m_b_abbr);
    tex2tex(chromatic, *buffer_a, *buffer_b);
    glShader::unbind();
    // -----------------------------------------------------------------------------------------


    // Color grading
    // -----------------------------------------------------------------------------------------
    glShader &colorgrade = getProgram("colorgrade");
    colorgrade.bind();
    colorgrade.set_float("un_gamma", m_gamma);
    colorgrade.set_float("un_exposure", m_exposure);
    tex2tex(colorgrade, *buffer_b, *buffer_a);
    glShader::unbind();
    // -----------------------------------------------------------------------------------------


    // FXAA
    // -----------------------------------------------------------------------------------------
    glShader &fxaa = getProgram("fxaa");
    fxaa.bind();
    f_fbfb(fxaa, *buffer_a);
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
    f_init_framebuffers(w, h);
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

