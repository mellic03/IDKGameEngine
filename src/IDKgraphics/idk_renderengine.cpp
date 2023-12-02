#include "idk_renderengine.h"

#include <filesystem>


// Static members -----------------------------
GLuint  idk::RenderEngine::SPHERE_PRIMITIVE;
GLuint  idk::RenderEngine::CUBE_PRIMITIVE;
GLuint  idk::RenderEngine::CRATE_PRIMITIVE;
// --------------------------------------------

void
idk::RenderEngine::init_SDL_OpenGL( std::string windowname, size_t w, size_t h, InitFlag flags )
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error creating window\n";
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


    Uint32 sdl_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (flags & InitFlag::INIT_HEADLESS) sdl_flags |= SDL_WINDOW_HIDDEN;

    m_SDL_window = SDL_CreateWindow(
        windowname.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        sdl_flags
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);


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
idk::RenderEngine::init_screenquad()
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
    createProgram("background",     "IDKGE/shaders/deferred/", "background.vs", "background.fs");
    createProgram("geometry_pass",  "IDKGE/shaders/deferred/", "geometrypass.vs", "geometrypass.fs");
    createProgram("geometry_light", "IDKGE/shaders/deferred/", "geometrypass.vs", "lightsource.fs");
    createProgram("lighting_ibl",   "IDKGE/shaders/", "screenquad.vs", "deferred/lightingpass_pbr.fs");
    createProgram("dirvolumetrics", "IDKGE/shaders/", "screenquad.vs", "deferred/volumetric_dirlight.fs");
    createProgram("bloom",          "IDKGE/shaders/", "screenquad.vs", "postprocess/bloom.fs");
    createProgram("downsample",     "IDKGE/shaders/", "screenquad.vs", "postprocess/downsample.fs");
    createProgram("upsample",       "IDKGE/shaders/", "screenquad.vs", "postprocess/upsample.fs");
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
idk::RenderEngine::init_framebuffers( int w, int h )
{
    idk::glColorConfig config = {
        .internalformat = GL_RGBA16F,
        .minfilter      = GL_NEAREST,
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

    config = {
        .internalformat = GL_RGBA16F,
        .minfilter      = GL_LINEAR_MIPMAP_LINEAR,
        .magfilter      = GL_LINEAR,
        .datatype       = GL_FLOAT,
        .genmipmap      = true
    };

    m_mainbuffer_1.reset(w, h, 1);
    m_mainbuffer_1.colorAttachment(0, config);
}


void
idk::RenderEngine::init_all( std::string name, int w, int h )
{
    RenderEngine::SPHERE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::icosphere, "");
    RenderEngine::CUBE_PRIMITIVE   = modelManager().loadOBJ(idk::objprimitives::cube, "");
    RenderEngine::CRATE_PRIMITIVE  = modelManager().loadOBJ(idk::objprimitives::crate, "");

    compileShaders();

    init_screenquad();
    init_framebuffers(w, h);

    m_lightsystem.init();
    m_UBO_pointlights = glUBO(3, 16 + IDK_MAX_POINTLIGHTS*sizeof(Pointlight));
    m_UBO_dirlights   = glUBO(5, IDK_MAX_DIRLIGHTS * (sizeof(Dirlight) + sizeof(glm::mat4)));
    BRDF_LUT = gltools::loadTexture("assets/IBL_BRDF_LUT.png", false, GL_LINEAR, GL_LINEAR);

    m_model_manager.init();
    RenderEngine::SPHERE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::icosphere, "");
    RenderEngine::CUBE_PRIMITIVE   = modelManager().loadOBJ(idk::objprimitives::cube, "");
    RenderEngine::CRATE_PRIMITIVE  = modelManager().loadOBJ(idk::objprimitives::crate, "");

    m_active_camera_id = createCamera();
    m_UBO_camera = glUBO(2, 2*sizeof(glm::mat4) + 6*sizeof(glm::vec4));

}


void
idk::RenderEngine::init( std::string name, int w, int h, InitFlag flags )
{
    m_resolution = glm::ivec2(w, h);

    init_SDL_OpenGL(name, w, h, flags);
    current_skybox = 0;

    if (flags == InitFlag::NONE)
    {
        init_all(name, w, h);
        return;
    }

    if (flags & InitFlag::INIT_PROGRAMS)
    {
        compileShaders();
    }

    if (flags & InitFlag::INIT_FRAMEBUFFERS)
    {
        init_screenquad();
        init_framebuffers(w, h);
    }

    if (flags & InitFlag::INIT_LIGHTING)
    {
        m_lightsystem.init();
        m_UBO_pointlights = glUBO(3, 16 + IDK_MAX_POINTLIGHTS*sizeof(Pointlight));
        m_UBO_dirlights   = glUBO(5, IDK_MAX_DIRLIGHTS * (sizeof(Dirlight) + sizeof(glm::mat4)));
        BRDF_LUT = gltools::loadTexture("assets/IBL_BRDF_LUT.png", false, GL_LINEAR, GL_LINEAR);
    }

    if (flags & InitFlag::INIT_MODELS)
    {
        m_model_manager.init();
        RenderEngine::SPHERE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::icosphere, "");
        RenderEngine::CUBE_PRIMITIVE   = modelManager().loadOBJ(idk::objprimitives::cube, "");
        RenderEngine::CRATE_PRIMITIVE  = modelManager().loadOBJ(idk::objprimitives::crate, "");
    }

    if (flags & InitFlag::INIT_CAMERA)
    {
        m_active_camera_id = createCamera();
        m_UBO_camera = glUBO(2, 2*sizeof(glm::mat4) + 6*sizeof(glm::vec4));
    }

    if (flags & InitFlag::INIT_NOISE)
    {
    }
}


GLuint
idk::RenderEngine::createProgram( std::string name, std::string root, std::string vs, std::string fs )
{
    m_shader_names.push_back(name);
    return m_shaders[name].loadFileC(root, vs, fs);
}


int
idk::RenderEngine::loadSkybox( const std::string &filepath )
{
    gl::enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    std::vector<std::string> faces
    {
        "px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png"
    };

    std::vector<std::string> faces2
    {
        "0px.png", "0nx.png", "0py.png", "0ny.png", "0pz.png", "0nz.png"
    };


    glColorConfig skybox_config = {
        .internalformat = GL_SRGB8_ALPHA8,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR,
        .magfilter      = GL_LINEAR,
        .datatype       = GL_UNSIGNED_BYTE,
        .genmipmap      = false
    };

    glColorConfig diffuse_config = {
        .internalformat = GL_SRGB8_ALPHA8,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR,
        .magfilter      = GL_LINEAR,
        .datatype       = GL_UNSIGNED_BYTE,
        .genmipmap      = true
    };

    glColorConfig specular_config = {
        .internalformat = GL_SRGB8_ALPHA8,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR_MIPMAP_LINEAR,
        .magfilter      = GL_LINEAR,
        .datatype       = GL_UNSIGNED_BYTE,
        .genmipmap      = false,
        .setmipmap      = true,
        .texbaselevel   = 0,
        .texmaxlevel    = 5
    };

    GLuint skybox   = gltools::loadCubemap(filepath, faces, skybox_config);
    GLuint diffuse  = gltools::loadCubemap(filepath + "diffuse/", faces, diffuse_config);
    GLuint specular = gltools::loadCubemap(filepath + "specular/", faces2, specular_config);

    for (GLint mip=0; mip<=5; mip++)
    {
        gltools::loadCubemapMip(filepath + "specular/", faces, specular_config, specular, mip);
    }

    skyboxes.push_back(skybox);
    skyboxes_IBL.push_back(std::make_pair(diffuse, specular));

    return skyboxes.size() - 1;
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
    m_model_draw_queue[shader_id].push_back({model_id, model_mat});
}


void
idk::RenderEngine::drawShadowCaster( int model_id, glm::mat4 &model_mat )
{
    m_shadowcast_queue.push_back({model_id, model_mat});
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
    m_pointlight_queue.push_back(light);
}



#define ORTHO_N 1.0f
#define ORTHO_F 50.0f
#define ORTHO_W 30.0f
#define ORTHO_R 0.1f

void
idk::RenderEngine::shadowpass_dirlights()
{
    idk::Camera &cam = getCamera();

    idk::glShader &program = getProgram("dir_shadow");
    program.bind();

    auto &queue  = m_model_draw_queue;
    auto quadVAO = m_quad_VAO;

    glm::mat4 projection = glm::ortho(-ORTHO_W, ORTHO_W, -ORTHO_W, ORTHO_W, ORTHO_N, ORTHO_F);

    std::vector<idk::Dirlight> &dirlights       = m_lightsystem.dirlights();
    std::vector<idk::glFramebuffer> &shadowmaps = m_lightsystem.shadowmaps();

    for (int i=0; i<dirlights.size(); i++)
    {
        shadowmaps[i].clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::vec3 dir = glm::normalize(glm::vec3(dirlights[i].direction));

        glm::mat4 view = glm::lookAt(
            cam.transform().position() - (ORTHO_N*ORTHO_F / 4.0f) * dir,
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
idk::RenderEngine::update_UBO_camera()
{
    idk::Camera &camera = getCamera();
    m_UBO_camera.bind();
    m_UBO_camera.add<glm::mat4>(glm::value_ptr(camera.view()));
    m_UBO_camera.add<glm::mat4>(glm::value_ptr(camera.projection()));
    m_UBO_camera.add<glm::vec3>(glm::value_ptr(camera.transform().position()));

    glm::vec4 b(camera.m_r_abr, camera.m_g_abr);
    glm::vec4 c(camera.m_b_abr, camera.m_abr_str, 0.0f);

    m_UBO_camera.add<glm::vec3>(glm::value_ptr(camera.transform().position()));
    m_UBO_camera.add<glm::vec4>(glm::value_ptr(camera.m_bloom_gamma));
    m_UBO_camera.add<glm::vec4>(glm::value_ptr(b));
    m_UBO_camera.add<glm::vec4>(glm::value_ptr(c));
    m_UBO_camera.add<glm::vec4>(glm::value_ptr(camera.m_exposure));

    m_UBO_camera.unbind();
}


void
idk::RenderEngine::update_UBO_dirlights()
{
    idk::Camera &cam = getCamera();

    std::vector<Dirlight>  lights(IDK_MAX_DIRLIGHTS);
    std::vector<glm::mat4> matrices(IDK_MAX_DIRLIGHTS);

    glm::mat4 projection = glm::ortho(-ORTHO_W, ORTHO_W, -ORTHO_W, ORTHO_W, ORTHO_N, ORTHO_F);
    std::vector<idk::Dirlight> &dirlights = m_lightsystem.dirlights();

    for (int i=0; i<dirlights.size(); i++)
    {
        const glm::vec3 dir = glm::normalize(glm::vec3(dirlights[i].direction));

        glm::mat4 view = glm::lookAt(
            cam.transform().position() - (ORTHO_N*ORTHO_F / 4.0f) * dir,
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
idk::RenderEngine::update_UBO_pointlights()
{
    idk::Camera &cam = getCamera();

    std::vector<Pointlight> &pointlights = m_lightsystem.pointlights();
    std::vector<Pointlight>  lights(IDK_MAX_POINTLIGHTS);

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

        glShader &lighting_ibl = getProgram("lighting_ibl");
        m_lightsystem.genShaderString(vert_source, frag_source);
        lighting_ibl.loadStringC(vert_source, frag_source);
    }

    gl::disable(GL_CULL_FACE);
    shadowpass_dirlights();
    m_shadowcast_queue.clear();
    gl::enable(GL_CULL_FACE);


    update_UBO_camera();
    update_UBO_dirlights();
    update_UBO_pointlights();


    idk::Camera &camera = getCamera();

    glShader::unbind();

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
    glm::mat4 modelmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.8f * camera.farPlane()));
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

    std::vector<glFramebuffer> &shadowmaps = m_lightsystem.shadowmaps();

    for (int i=0; i<m_lightsystem.dirlights().size(); i++)
    {
        lighting.set_sampler2D(
            "un_dirlight_depthmaps[" + std::to_string(i) + "]", shadowmaps[i].attachments[0]
        );
    }

    lighting.set_samplerCube("un_skybox_diffuse",  skyboxes_IBL[current_skybox].first);
    lighting.set_samplerCube("un_skybox_specular", skyboxes_IBL[current_skybox].second);

    tex2tex(lighting, m_deferred_geom_buffer, m_scratchbufs1[0]);

    lighting.popTextureUnits();
    glShader::unbind();
    // -----------------------------------------------------------------------------------------



    // Volumetric directional lights
    // -----------------------------------------------------------------------------------------
    // glShader &dvol = m_shaders["dirvolumetrics"];
    // dvol.bind();
    // for (int i=0; i<m_lightsystem.dirlights().size(); i++)
    // {
    //     dvol.set_sampler2D(
    //         "un_dirlight_depthmaps[" + std::to_string(i) + "]", shadowmaps[i].attachments[0]
    //     );
    // }

    // tex2tex(dvol, m_deferred_geom_buffer, m_scratchbufs0[2]);
    // glShader::unbind();
    // -----------------------------------------------------------------------------------------


    // Combine geometry and volumetrics
    // -----------------------------------------------------------------------------------------
    glShader &additive = getProgram("additive");
    // additive.bind();
    // additive.set_float("intensity", 1.0f);
    // tex2tex(additive, m_scratchbufs0[0], m_scratchbufs0[2], m_scratchbufs1[0]);
    // glShader::unbind();
    // -----------------------------------------------------------------------------------------


    // Bloom
    // -----------------------------------------------------------------------------------------
    glShader bloom = getProgram("bloom");
    bloom.bind();
    tex2tex(bloom, m_scratchbufs1[0], m_scratchbufs2[0]);

    glShader &downsample = getProgram("downsample");
    glShader &upsample   = getProgram("upsample");

    constexpr int miplevel = 7;

    downsample.bind();
    tex2tex(downsample, m_scratchbufs2[0], m_scratchbufs1[1]);
    for (int i=1; i<miplevel; i++)
    {
        tex2tex(downsample, m_scratchbufs1[i], m_scratchbufs1[i+1]);
    }

    upsample.bind();
    tex2tex(upsample, m_scratchbufs1[miplevel], m_scratchbufs1[miplevel], m_scratchbufs2[miplevel-1]);
    for (int i=miplevel-1; i>0; i--)
    {
        tex2tex(upsample, m_scratchbufs1[i], m_scratchbufs2[i], m_scratchbufs2[i-1]);
    }

    additive.bind();
    additive.set_float("intensity", getCamera().m_bloom_gamma.x);
    tex2tex(additive, m_scratchbufs1[0], m_scratchbufs2[0], m_mainbuffer_0);

    glShader::unbind();
    // -----------------------------------------------------------------------------------------


    glFramebuffer *buffer_a = &m_mainbuffer_0;
    glFramebuffer *buffer_b = &m_mainbuffer_1;


    // Chromatic aberration
    // -----------------------------------------------------------------------------------------
    glShader &chromatic = getProgram("chromatic");
    chromatic.bind();
    tex2tex(chromatic, *buffer_a, *buffer_b);
    glShader::unbind();
    // -----------------------------------------------------------------------------------------


    buffer_b->generateMipmap(0);


    // Color grading
    // -----------------------------------------------------------------------------------------
    glShader &colorgrade = getProgram("colorgrade");
    colorgrade.bind();

    const GLint secondmip = 4;
    const GLint level  = log2(GL_MAX_TEXTURE_SIZE);
    const GLint level2 = level - secondmip;

    const size_t size   = 4;
    const size_t size2  = size * pow(4, secondmip);
    static float *data  = new float[size];
    static float *data2 = new float[size2];

    GLCALL( glGetTextureImage(buffer_b->attachments[0], level,  GL_RGBA, GL_FLOAT, size*sizeof(float),  data); )
    GLCALL( glGetTextureImage(buffer_b->attachments[0], level2, GL_RGBA, GL_FLOAT, size2*sizeof(float), data2); )
 
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

    printf("%.0f  %.0f  %.0f  %.0f  %u  %u\n", texw, texh, mipw, miph, center_x, center_y);
    
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

    // camera.m_exposure.x = a*camera.m_exposure.x + b*new_exposure;

    printf("avg: %.2f  %.2f  %.2f\n", avg_color.x, avg_color.y, avg_color.z);
    printf("luminance:  %.2f\n",   avg_luminance);
    printf("exposure:   %.2f\n\n", camera.m_exposure.x);


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
    init_framebuffers(w, h);
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

