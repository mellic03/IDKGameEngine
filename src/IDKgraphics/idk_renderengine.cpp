#include "idk_renderengine.h"

#include <filesystem>


// Static methods -------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------




// Static members -----------------------------
GLuint  idk::RenderEngine::SPHERE_PRIMITIVE;
GLuint  idk::RenderEngine::CUBE_PRIMITIVE;
// --------------------------------------------

void
idk::RenderEngine::_init_SDL_OpenGL( std::string windowname, size_t w, size_t h)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error creating window\n";
        exit(1);
    }

    _SDL_window = SDL_CreateWindow(
        windowname.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

    _SDL_gl_context = SDL_GL_CreateContext(_SDL_window);
    SDL_GL_MakeCurrent(_SDL_window, _SDL_gl_context);
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
idk::RenderEngine::_init_screenquad()
{
    float quad_vertices[] = {
      -1.0f,  1.0f,  0.0f,  1.0f,
      -1.0f, -1.0f,  0.0f,  0.0f,
       1.0f, -1.0f,  1.0f,  0.0f,

      -1.0f,  1.0f,  0.0f,  1.0f,
       1.0f, -1.0f,  1.0f,  0.0f,
       1.0f,  1.0f,  1.0f,  1.0f
    };

    // Send screen quad to GPU
    // ------------------------------------------------------------------------------------
    gl::genVertexArrays(1, &_quad_VAO);
    gl::genBuffers(1, &_quad_VBO);

    gl::bindVertexArray(_quad_VAO);
    gl::bindBuffer(GL_ARRAY_BUFFER, _quad_VBO);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    gl::vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    gl::enableVertexAttribArray(0);

    gl::vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 2*sizeof(float));
    gl::enableVertexAttribArray(1);
    // ------------------------------------------------------------------------------------
}


idk::RenderEngine::RenderEngine( std::string windowname, size_t w, size_t h ):
_res_x(w), _res_y(h),
_deferred_geometrypass_buffer(4), _screenquad_buffer(1), _colorgrade_buffer(1),
_dirlight_depthmap_buffer(1)
{
    glInterface::init();
    _init_SDL_OpenGL(windowname, _res_x, _res_y);
    _init_screenquad();

    _active_camera_id = createCamera();

    RenderEngine::CUBE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::cube, "");
    RenderEngine::SPHERE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::icosphere, "");

    glInterface::genIdkFramebuffer(_res_x, _res_y, _deferred_geometrypass_buffer);
    glInterface::genIdkFramebuffer(_res_x, _res_y, _screenquad_buffer);
    glInterface::genIdkFramebuffer(_res_x, _res_y, _colorgrade_buffer);
    glInterface::genIdkFramebuffer(2048, 2048, _dirlight_depthmap_buffer);

    _deferred_geometrypass_shader = glInterface::compileProgram(
        "assets/shaders/deferred/", "geometrypass.vs", "geometrypass.fs"
    );
    _deferred_lightingpass_shader = glInterface::compileProgram(
        "assets/shaders/deferred/", "lightingpass.vs", "lightingpass.fs"
    );

    _dirshadow_shader = glInterface::compileProgram("assets/shaders/", "dirshadow.vs", "dirshadow.fs");
    _screenquad_shader = glInterface::compileProgram("assets/shaders/", "screenquad.vs", "screenquad.fs");
    _colorgrade_shader = glInterface::compileProgram("assets/shaders/", "screenquad.vs", "postprocess/colorgrade.fs");
    _fxaa_shader = glInterface::compileProgram("assets/shaders/", "screenquad.vs", "postprocess/fxaa.fs");
    solid_shader = glInterface::compileProgram("assets/shaders/", "vsin_pos_only.vs", "solid.fs");

    _UBO_camera      = glUBO(2, 2*sizeof(glm::mat4) + sizeof(glm::vec4));
    _UBO_pointlights = glUBO(3, 16 + IDK_MAX_POINTLIGHTS*sizeof(lightsource::Point));
    _UBO_spotlights  = glUBO(4, 16 + IDK_MAX_SPOTLIGHTS*sizeof(lightsource::Spot));
    _UBO_dirlights   = glUBO(5, 16 + IDK_MAX_DIRLIGHTS*sizeof(lightsource::Dir));
}


void
idk::RenderEngine::_render_screenquad( GLuint shader, glFramebuffer &in, glFramebuffer &out )
{
    glInterface::bindIdkFramebuffer(out);
    glInterface::useProgram(shader);

    for (int i=0; i < in.textures.size(); i++)
    {
        std::string name = "un_texture_" + std::to_string(i);
        glInterface::setUniform_texture(name, in.textures[i]);
    }

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


void
idk::RenderEngine::_render_screenquad( GLuint shader, glFramebuffer &in )
{
    glInterface::unbindIdkFramebuffer(_res_x, _res_y);
    glInterface::useProgram(shader);

    for (int i=0; i < in.textures.size(); i++)
        glInterface::setUniform_texture("un_texture_" + std::to_string(i), in.textures[0]);

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


int
idk::RenderEngine::createCamera()
{
    int camera_id = _camera_allocator.add();
    _camera_allocator.get(camera_id).aspect(_res_x, _res_y);
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
    glInterface::bindIdkFramebuffer(_dirlight_depthmap_buffer);
    glInterface::useProgram(_dirshadow_shader);

    float near_plane = 1.0f, far_plane = 50.0f;

    RenderEngine &ren = *this;
    auto &queue = _model_draw_queue;
    auto &depthmaps = _dirlight_shadowmap_allocator;
    auto &framebuffer = _dirlight_depthmap_buffer;
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
        glInterface::setUniform_mat4("un_dirlight_projection", projection);
        glInterface::setUniform_mat4("un_dirlight_view", view);

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

        depthmaps.get(light_id) = framebuffer.textures[0];
        lmats.get(light_id) = projection * view;

        i += 1;
    });

    glInterface::clearIdkFramebuffer(_dirlight_depthmap_buffer);
}


void
idk::RenderEngine::_update_UBO_camera()
{
    idk::Camera &camera = getCamera();
    _UBO_camera.bind();
    _UBO_camera.add<glm::mat4>(glm::value_ptr(camera.view()));
    _UBO_camera.add<glm::mat4>(glm::value_ptr(camera.projection()));
    _UBO_camera.add<glm::vec3>(glm::value_ptr(camera.transform().position()));
    _UBO_camera.unbind();
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

    _UBO_pointlights.bind();
    _UBO_pointlights.add<int>(&num_pointlights);
    _UBO_pointlights.add(sizeof(lightsource::Point)*IDK_MAX_SPOTLIGHTS, &(lights[0]));
    _UBO_pointlights.unbind();
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

    _UBO_spotlights.bind();
    _UBO_spotlights.add<int>(&num_spotlights);
    _UBO_spotlights.add(sizeof(lightsource::Spot)*IDK_MAX_SPOTLIGHTS, &(lights[0]));
    _UBO_spotlights.unbind();
}


void
idk::RenderEngine::_update_UBO_dirlights()
{
    int num_dirlights = dirlights().size();

    std::vector<idk::lightsource::Dir> lights;
    dirlights().for_each(
        [&lights](lightsource::Dir &light)
        {
            lights.push_back(light);
        }
    );
    lights.resize(IDK_MAX_DIRLIGHTS);

    _UBO_dirlights.bind();
    _UBO_dirlights.add<int>(&num_dirlights);
    _UBO_dirlights.add(sizeof(lightsource::Dir)*IDK_MAX_DIRLIGHTS, &(lights[0]));
    _UBO_dirlights.unbind();
}


void
idk::RenderEngine::beginFrame()
{
    SDL_GL_SwapWindow(_SDL_window);

    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);
    gl::clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    glInterface::bindIdkFramebuffer(_deferred_geometrypass_buffer);

    for (auto &[shader_id, vec]: _model_draw_queue)
    {
        glInterface::useProgram(shader_id);

        // glInterface::setUniform_texture("un_dirlight_depthmaps[0]", _dirlight_shadowmap_allocator.get(0));
        // glInterface::setUniform_mat4("un_dirlight_lightspacematrices[0]", _dirlight_lightspacematrix_allocator.get(0));

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

    // for (auto &[shader_id, vec]: _untextured_model_queue)
    // {
    //     glInterface::useProgram(shader_id);
    //     for (auto &[model_id, transform]: vec)
    //     {
    //         drawmethods::draw_untextured(
    //             modelManager().getModel(model_id),
    //             transform,
    //             modelManager().getMaterials()
    //         );
    //         glInterface::freeTextureUnitIDs();
    //     }
    //     vec.clear();
    // }
    // _untextured_model_queue.clear();
    gl::bindVertexArray(0);
    // -------------------------------------------------------------------


    // Deferred lighting pass --------------------------------------------
    gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);
    _render_screenquad(_deferred_lightingpass_shader, _deferred_geometrypass_buffer, _screenquad_buffer);
    _render_screenquad(_fxaa_shader, _screenquad_buffer, _colorgrade_buffer);
    _render_screenquad(_colorgrade_shader, _colorgrade_buffer);
    gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
    // -------------------------------------------------------------------

    glInterface::clearIdkFramebuffers(
        _deferred_geometrypass_buffer,
        _screenquad_buffer,
        _colorgrade_buffer
    );
}


void
idk::RenderEngine::resize( int w, int h )
{
    _res_x = w;
    _res_y = h;

    glInterface::genIdkFramebuffer(w, h, _deferred_geometrypass_buffer);
    glInterface::genIdkFramebuffer(w, h, _screenquad_buffer);
    glInterface::genIdkFramebuffer(w, h, _colorgrade_buffer);

    getCamera().aspect(w, h);
}

