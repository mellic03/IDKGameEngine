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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


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
    // Generate screen quad vertex array
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
_gbuffer_geometrypass(4)
{
    _init_SDL_OpenGL(w, h);
    _init_screenquad();

    _gl_interface.genScreenBuffer(_gbuffer_geometrypass);
    _screenquad_shader = compileShaderProgram("assets/shaders/", "screenquad.vs", "mouse.fs");

}


GLuint
idk::RenderEngine::compileShaderProgram(std::string root, std::string vs, std::string fs)
{
    GLuint id = _gl_interface.compileShaderProgram(root, vs, fs);
    _glShader_allocator[id] = glShader();
    return id;
}


void
idk::RenderEngine::drawModel(uint model_id, uint transform_id)
{
    _model_draw_queue[_active_shader_id].push({model_id, transform_id, _active_glShader_id});
}



uint
idk::RenderEngine::loadOBJ(std::string root, std::string obj, std::string mtl)
{
    return _model_allocator.add(idk::Model(root, obj, mtl));
}


void
idk::RenderEngine::beginFrame()
{
    _gl_interface.free_glTextureUnitIDs();

    GLCALL( glClearColor(0.0f, 0.0f, 0.0f, 1.0f); )
    GLCALL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); )
}


void
idk::RenderEngine::endFrame()
{
    GLuint geometrypass_framebuffer = 0;
    // _gl_interface.bind_framebuffer(geometrypass_framebuffer);

    for (auto &[shader_id, vec]: _model_draw_queue)
    {
        _gl_interface.bindShaderProgram(shader_id);
        for (auto [model_id, transform_id, glShader_id]: vec)
        {
            _gl_interface.draw_model(
                _model_allocator.get(model_id),
                _transform_allocator.get(transform_id),
                _glShader_allocator[glShader_id]
            );
        }
        vec.clear();
    }

    // Draw final texture to the screen quad
    _gl_interface.free_glTextureUnitIDs();
    // _gl_interface.bindShaderProgram(_screenquad_shader);

    GLCALL( glDisable(GL_DEPTH_TEST); )
    GLCALL( glDisable(GL_CULL_FACE); )

    GLCALL( glBindVertexArray(_quad_VAO); )
    GLCALL( glDrawArrays(GL_TRIANGLES, 0, 6); )
    GLCALL( glBindVertexArray(0); )

    SDL_GL_SwapWindow(_SDL_window);
}

