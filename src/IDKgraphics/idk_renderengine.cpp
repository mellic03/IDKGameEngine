#include "idk_renderengine.h"

#include <filesystem>


// Static members -----------------------------
GLuint  idk::RenderEngine::SPHERE_PRIMITIVE;
GLuint  idk::RenderEngine::CUBE_PRIMITIVE;
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
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
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
    gl::genVertexArrays(1, &_quad_VAO);
    gl::genBuffers(1, &_quad_VBO);

    gl::bindVertexArray(_quad_VAO);
    gl::bindBuffer(GL_ARRAY_BUFFER, _quad_VBO);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    gl::vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    gl::enableVertexAttribArray(0);

    gl::vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 3*sizeof(float));
    gl::enableVertexAttribArray(1);
    // ------------------------------------------------------------------------------------
}


void
idk::RenderEngine::f_compile_shaders()
{
    std::cout << "compiling shaders\n";

    m_background_shader = glInterface::compileProgram(
        "shaders/deferred/", "background.vs", "background.fs"
    );
    m_lighting_background_shader = glInterface::compileProgram(
        "shaders/deferred/", "lightingpass.vs", "lightingpass_background.fs"
    );
    m_deferred_geometrypass_shader = glInterface::compileProgram(
        "shaders/deferred/", "geometrypass.vs", "geometrypass.fs"
    );
    m_deferred_lightingpass_shader = glInterface::compileProgram(
        "shaders/deferred/", "lightingpass.vs", "lightingpass.fs"
    );
    m_dirlight_vol_shader = glInterface::compileProgram(
        "shaders/deferred/", "screenquad.vs", "volumetric_dirlight.fs"
    );

    _additive_shader   = glInterface::compileProgram("shaders/", "screenquad.vs", "postprocess/additive.fs");
    _colorgrade_shader = glInterface::compileProgram("shaders/", "screenquad.vs", "postprocess/colorgrade.fs");
    _fxaa_shader       = glInterface::compileProgram("shaders/", "screenquad.vs", "postprocess/fxaa.fs");
    _dirshadow_shader  = glInterface::compileProgram("shaders/", "dirshadow.vs", "dirshadow.fs");
    solid_shader       = glInterface::compileProgram("shaders/", "vsin_pos_only.vs", "solid.fs");
}


void
idk::RenderEngine::f_gen_idk_framebuffers( int w, int h )
{
    m_deferred_geom_buffer      = glInterface::genIdkFramebuffer(    w,     h,   4);
    m_volumetrics_buffer        = glInterface::genIdkFramebuffer(  w/4,   h/4,   1);
    m_colorgrade_buffer         = glInterface::genIdkFramebuffer(    w,     h,   1);
    m_fxaa_buffer               = glInterface::genIdkFramebuffer(    w,     h,   1);
    m_final_buffer              = glInterface::genIdkFramebuffer(    w,     h,   1);
    m_dirlight_depthmap_buffer  = glInterface::genIdkFramebuffer( 2048,  2048,   1);
}



idk::RenderEngine::RenderEngine( std::string name, int w, int h, int res_divisor )
{
    glInterface::init();

    m_resolution = glm::ivec2(w, h);
    f_init_SDL_OpenGL(name, w, h);
    f_init_screenquad();

    RenderEngine::CUBE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::cube, "");
    RenderEngine::SPHERE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::icosphere, "");
   
    f_gen_idk_framebuffers(w, h);
    f_compile_shaders();

    m_active_camera_id = createCamera();

    m_UBO_camera      = glUBO(2, 2*sizeof(glm::mat4) + sizeof(glm::vec4));
    m_UBO_pointlights = glUBO(3, 16 + IDK_MAX_POINTLIGHTS*sizeof(lightsource::Point));
    m_UBO_spotlights  = glUBO(4, 16 + IDK_MAX_SPOTLIGHTS*sizeof(lightsource::Spot));
    m_UBO_dirlights   = glUBO(5, 16 + IDK_MAX_DIRLIGHTS * (sizeof(lightsource::Dir) + sizeof(glm::mat4)));
}


void
idk::RenderEngine::_render_screenquad( GLuint shader, glFramebuffer &in, glFramebuffer &out )
{
    glInterface::bindIdkFramebuffer(out);
    glInterface::useProgram(shader);

    glInterface::setUniform_texture("un_dirlight_depthmaps[0]", _dirlight_shadowmap_allocator.get(0));

    for (int i=0; i < in.output_textures.size(); i++)
    {
        std::string name = "un_texture_" + std::to_string(i);
        glInterface::setUniform_texture(name, in.output_textures[i]);
    }

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


void
idk::RenderEngine::_render_screenquad( GLuint shader, glFramebuffer &in )
{
    glInterface::unbindIdkFramebuffer(m_resolution.x, m_resolution.y);
    glInterface::useProgram(shader);

    for (int i=0; i < in.output_textures.size(); i++)
        glInterface::setUniform_texture("un_texture_" + std::to_string(i), in.output_textures[0]);

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


void
idk::RenderEngine::_render_screenquad( GLuint shader, GLuint tex0, GLuint tex1 )
{
    glInterface::unbindIdkFramebuffer(m_resolution.x, m_resolution.y);
    glInterface::useProgram(shader);

    glInterface::setUniform_texture("un_texture_0", tex0);
    glInterface::setUniform_texture("un_texture_1", tex1);

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


void
idk::RenderEngine::_render_screenquad( GLuint shader, GLuint tex0, GLuint tex1, glFramebuffer &out )
{
    glInterface::bindIdkFramebuffer(out);
    glInterface::useProgram(shader);

    glInterface::setUniform_texture("un_dirlight_depthmaps[0]", _dirlight_shadowmap_allocator.get(0));

    glInterface::setUniform_texture("un_texture_0", tex0);
    glInterface::setUniform_texture("un_texture_1", tex1);

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


int
idk::RenderEngine::createCamera()
{
    int camera_id = _camera_allocator.add();
    _camera_allocator.get(camera_id).aspect(m_resolution.x, m_resolution.y);
    return camera_id;
}


int
idk::RenderEngine::createPointlight()
{
    return _pointlight_allocator.add();
}


int
idk::RenderEngine::createSpotlight()
{
    return _spotlight_allocator.add();
}


int
idk::RenderEngine::createDirlight()
{
    _dirlight_shadowmap_allocator.add();
    _dirlight_lightspacematrix_allocator.add();
    return _dirlight_allocator.add();
}


void
idk::RenderEngine::drawModel( GLuint shader_id, int model_id, Transform &transform )
{
    _model_draw_queue[shader_id].push({model_id, transform});
}

void
idk::RenderEngine::drawUntextured( GLuint shader_id, int model_id, Transform &transform )
{
    // _untextured_model_queue[shader_id].push({model_id, transform});
}


void
idk::RenderEngine::_shadowpass_pointlights()
{

}


void
idk::RenderEngine::_shadowpass_spotlights()
{

}


void
idk::RenderEngine::_shadowpass_dirlights()
{
    // glInterface::clearIdkFramebuffer(m_dirlight_depthmap_buffer);
    glInterface::bindIdkFramebuffer(m_dirlight_depthmap_buffer);
    glInterface::useProgram(_dirshadow_shader);

    float near_plane = 1.0f, far_plane = 50.0f;

    RenderEngine &ren = *this;
    auto &queue = _model_draw_queue;
    auto &depthmaps = _dirlight_shadowmap_allocator;
    auto &framebuffer = m_dirlight_depthmap_buffer;
    auto &lmats = _dirlight_lightspacematrix_allocator;

    int i = 0;
    dirlights().for_each(
    [&i, &ren, &queue, &depthmaps, &lmats, &framebuffer](int light_id, lightsource::Dir &light)
    {
        glm::mat4 projection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 50.0f);
        glm::mat4 view = glm::lookAt(
           -10.0f*glm::vec3(light.direction) + ren.getCamera().transform().position(),
            glm::vec3(0.0f) + ren.getCamera().transform().position(),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
    
        glm::mat4 lightspacematrix = projection * view;
        glInterface::setUniform_mat4("un_lightspacematrix", lightspacematrix);

        for (auto &[shader_id, vec]: queue)
        {
            for (auto &[model_id, transform]: vec)
            {
                drawmethods::draw_untextured(
                    ren.modelManager().getModel(model_id),
                    transform,
                    ren.modelManager().getMaterials()
                );
            }
        }

        depthmaps.get(light_id) = framebuffer.output_textures[0];
        lmats.get(light_id) = lightspacematrix;

        i += 1;
    });
}


void
idk::RenderEngine::_update_UBO_camera()
{
    idk::Camera &camera = getCamera();
    m_UBO_camera.bind();
    m_UBO_camera.add<glm::mat4>(glm::value_ptr(camera.view()));
    m_UBO_camera.add<glm::mat4>(glm::value_ptr(camera.projection()));
    m_UBO_camera.add<glm::vec3>(glm::value_ptr(camera.transform().position()));
    m_UBO_camera.unbind();
}


void
idk::RenderEngine::_update_UBO_pointlights()
{
    int num_pointlights = pointlights().size();

    std::vector<idk::lightsource::Point> lights;
    pointlights().for_each(
        [&lights](lightsource::Point &light)
        {
            lights.push_back(light);
        }
    );
    lights.resize(IDK_MAX_POINTLIGHTS);

    m_UBO_pointlights.bind();
    m_UBO_pointlights.add<int>(&num_pointlights);
    m_UBO_pointlights.add(sizeof(lightsource::Point)*IDK_MAX_SPOTLIGHTS, &(lights[0]));
    m_UBO_pointlights.unbind();
}


void
idk::RenderEngine::_update_UBO_spotlights()
{
    int num_spotlights = spotlights().size();

    std::vector<idk::lightsource::Spot> lights;
    spotlights().for_each(
        [&lights](lightsource::Spot &light)
        {
            lights.push_back(light);
        }
    );
    lights.resize(IDK_MAX_SPOTLIGHTS);

    m_UBO_spotlights.bind();
    m_UBO_spotlights.add<int>(&num_spotlights);
    m_UBO_spotlights.add(sizeof(lightsource::Spot)*IDK_MAX_SPOTLIGHTS, &(lights[0]));
    m_UBO_spotlights.unbind();
}


void
idk::RenderEngine::_update_UBO_dirlights()
{
    int num_dirlights = dirlights().size();

    std::vector<idk::lightsource::Dir> lights;
    std::vector<glm::mat4> matrices;

    dirlights().for_each(
        [&lights](lightsource::Dir &light)
        {
            lights.push_back(light);
        }
    );
    _dirlight_lightspacematrix_allocator.for_each(
        [&matrices](glm::mat4 &mat)
        {
            matrices.push_back(mat);
        }
    );

    lights.resize(IDK_MAX_DIRLIGHTS);
    matrices.resize(IDK_MAX_DIRLIGHTS);

    m_UBO_dirlights.bind();
    m_UBO_dirlights.add<int>(&num_dirlights);
    m_UBO_dirlights.add(sizeof(lightsource::Dir)*IDK_MAX_DIRLIGHTS, &(lights[0]));
    m_UBO_dirlights.add(sizeof(glm::mat4)*IDK_MAX_DIRLIGHTS, &(matrices[0]));
    m_UBO_dirlights.unbind();
}


void
idk::RenderEngine::beginFrame()
{
    glInterface::unbindIdkFramebuffer(m_resolution.x, m_resolution.y);
}


void
idk::RenderEngine::endFrame()
{
    _shadowpass_pointlights();
    _shadowpass_spotlights();
    _shadowpass_dirlights();

    _update_UBO_camera();
    _update_UBO_pointlights();
    _update_UBO_spotlights();
    _update_UBO_dirlights();

    idk::Camera &camera = getCamera();

    // Deferred geometry pass --------------------------------------------
    glInterface::bindIdkFramebuffer(m_deferred_geom_buffer);

    for (auto &[shader_id, vec]: _model_draw_queue)
    {
        glInterface::useProgram(shader_id);

        for (auto &[model_id, transform]: vec)
        {
            drawmethods::draw_textured(
                modelManager().getModel(model_id),
                transform,
                modelManager().getMaterials()
            );
            glInterface::freeTextureUnitIDs();
        }
        vec.clear();
    }
    _model_draw_queue.clear();
    gl::bindVertexArray(0);
    // -------------------------------------------------------------------

    // Deferred lighting pass --------------------------------------------
    glInterface::useProgram(m_background_shader);
    glm::mat4 modelmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.9f * camera.farPlane()));
    modelmat = glm::scale(modelmat, glm::vec3(300.0f, 300.0f, 1.0f));
    glInterface::setUniform_mat4("un_model", modelmat);

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);
    gl::bindVertexArray(0);

    gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);

    // Blinn-Phong lighting
    _render_screenquad(
        m_deferred_lightingpass_shader,
        m_deferred_geom_buffer,
        m_final_buffer
    );

    // Volumetric directional lights
    _render_screenquad(
        m_dirlight_vol_shader,
        m_deferred_geom_buffer,
        m_volumetrics_buffer
    );

    // Combine Blinn-Phong and volumetrics    
    _render_screenquad(
        _additive_shader,
        m_final_buffer.output_textures[0],
        m_volumetrics_buffer.output_textures[0],
        m_colorgrade_buffer
    );

    // Post processing
    _render_screenquad(_colorgrade_shader, m_colorgrade_buffer, m_fxaa_buffer);
    _render_screenquad(_fxaa_shader, m_fxaa_buffer);

    gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
    // -------------------------------------------------------------------

    glInterface::clearIdkFramebuffers(
        m_deferred_geom_buffer,
        m_volumetrics_buffer,
        m_fxaa_buffer,
        m_colorgrade_buffer,
        m_final_buffer
    );
}


void
idk::RenderEngine::swapWindow()
{
    SDL_GL_SwapWindow(m_SDL_window);
}


void
idk::RenderEngine::resize( int w, int h )
{
    m_resolution = { w, h };
    f_gen_idk_framebuffers(w, h);
    getCamera().aspect(w, h);
}

