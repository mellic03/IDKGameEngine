#include "IDK_renderengine.h"


void
idk::RenderEngine::_init_SDL_OpenGL(size_t w, size_t h)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error creating window\n";
        exit(1);
    }

    _SDL_window = SDL_CreateWindow(
        "IDK Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        SDL_WINDOW_OPENGL
        // SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}


void
idk::RenderEngine::_init_screenquad()
{
    // Send screen quad to GPU
    GLCALL( glGenVertexArrays(1, &_quad_VAO); )
    GLCALL( glGenBuffers(1, &_quad_VBO); )
    GLCALL( glBindVertexArray(_quad_VAO); )
    GLCALL( glBindBuffer(GL_ARRAY_BUFFER, _quad_VBO); )
    GLCALL( glBufferData(GL_ARRAY_BUFFER, sizeof(_quad_vertices), &_quad_vertices, GL_STATIC_DRAW); )

    GLCALL( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); )
    GLCALL( glEnableVertexAttribArray(0); )
    GLCALL( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); )
    GLCALL( glEnableVertexAttribArray(2); )
}


idk::RenderEngine::RenderEngine(size_t w, size_t h):
_screen_width(w), _screen_height(h),
_gbuffer_geometrypass(4), _screenquad_buffer(1)
{
    _init_SDL_OpenGL(_screen_width, _screen_height);
    _init_screenquad();

    idk::glInterface gl = _gl_interface;

    gl.genScreenBuffer(_screen_width, _screen_height, _gbuffer_geometrypass);
    _gbuffer_geometrypass_shader = gl.compileShaderProgram("assets/shaders/", "gb_geom.vs", "gb_geom.fs");

    _gl_interface.genScreenBuffer(_screen_width, _screen_height, _screenquad_buffer);
    _screenquad_shader = gl.compileShaderProgram("assets/shaders/", "screenquad.vs", "screenquad.fs");
}


void
idk::RenderEngine::_render_screenquad()
{
    idk::glInterface &gl = _gl_interface;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    gl.bindShaderProgram(_screenquad_shader);
    gl.setUniform_texture("un_screen_texture", _gbuffer_geometrypass.textures[0]);

    GLCALL( glDisable(GL_DEPTH_TEST); )
    GLCALL( glDisable(GL_CULL_FACE); )

    GLCALL( glBindVertexArray(_quad_VAO); )
    GLCALL( glDrawArrays(GL_TRIANGLES, 0, 6); )
    GLCALL( glBindVertexArray(0); )

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}


uint
idk::RenderEngine::createCamera()
{
    uint camera_id = _camera_allocator.add();
    _camera_allocator.get(camera_id).aspect(_screen_width, _screen_height);
    return camera_id;
}


// void
// idk::RenderEngine::controlCamera(std::function<void(idk::Keylog &, idk::Camera &)> fn)
// {
//     _camera_control_lambda = fn;
// }



idk::lightsource::Point &
idk::RenderEngine::createPointLight()
{
    uint transform_id = _transform_allocator.add();
    lightsource::Point point(transform_id);
    uint pointlight_id = _pointlight_allocator.add(point);
    return _pointlight_allocator.get(pointlight_id);
}


void
idk::RenderEngine::bindModel(uint model_id, uint transform_id)
{
    _model_draw_queue[_active_shader_id].push({model_id, transform_id, idk::glUniforms()});
    _active_glUniforms = &_model_draw_queue[_active_shader_id].back().third;
}


uint
idk::RenderEngine::loadOBJ(std::string root, std::string obj, std::string mtl)
{
    uint model_id = _model_allocator.add(idk::Model(root, obj, mtl));
    return model_id;
}


void
idk::RenderEngine::beginFrame()
{
    SDL_GL_SwapWindow(_SDL_window);
    _gl_interface.free_glTextureUnitIDs();

    GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, 0); )
    GLCALL( glClearColor(1.0f, 1.0f, 1.0f, 1.0f); )
    GLCALL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); )
}


void
idk::RenderEngine::endFrame()
{
    idk::Camera &camera = _camera_allocator.get(_active_camera_id);
    
    idk::glInterface &gl = _gl_interface;
    gl.bindScreenbuffer(_screen_width, _screen_height, _gbuffer_geometrypass);

    auto &transform_allocator = _transform_allocator;

    for (auto &[shader_id, vec]: _model_draw_queue)
    {
        gl.bindShaderProgram(shader_id);

        gl.setint("un_num_pointlights", _pointlight_allocator.size());

        int count = 0;
        _pointlight_allocator.for_each(
            [&gl, &transform_allocator, &count](lightsource::Point &pointlight)
            {
                idk::transform &transform = transform_allocator.get(pointlight.transform_id);
                std::string str = std::to_string(count);
                gl.setvec3(std::string("un_pointlights[" + str + "].ambient").c_str(), pointlight.ambient);
                gl.setvec3(std::string("un_pointlights[" + str + "].diffuse").c_str(), pointlight.diffuse);
                gl.setvec3(std::string("un_pointlights[" + str + "].position").c_str(), transform.position());

                gl.setfloat(std::string("un_pointlights[" + str + "].attenuation_constant").c_str(), pointlight.attentuation_constant);
                gl.setfloat(std::string("un_pointlights[" + str + "].attentuation_linear").c_str(), pointlight.attentuation_linear);
                gl.setfloat(std::string("un_pointlights[" + str + "].attentuation_quadratic").c_str(), pointlight.attentuation_quadratic);

                count += 1;
            }
        );

        glm::mat4 view = camera.view();
        glm::mat4 proj = camera.projection();

        gl.setvec3("un_viewpos", camera.transform().position());
        gl.setmat4("un_view", view);
        gl.setmat4("un_projection", proj);

        for (auto [model_id, transform_id, uniforms]: vec)
        {
            gl.draw_model(
                _model_allocator.get(model_id),
                _transform_allocator.get(transform_id),
                uniforms
            );
        }
        vec.clear();
    }
    _model_draw_queue.clear();
    _render_screenquad();
}

