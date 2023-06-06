#include "IDK_renderengine.h"



idk::RenderEngine::RenderEngine(size_t w, size_t h)
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
        1000,
        1000,
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


    _screenquad_shader = compileShaderProgram("assets/shaders/", "screenquad.vs", "mouse.fs");
}


GLuint
idk::RenderEngine::compileShaderProgram(std::string root, std::string vs, std::string fs)
{
    return _gl_interface.compileShaderProgram(root, vs, fs);
}


void
idk::RenderEngine::drawModel(int model_id, int transform_id)
{
    _model_draw_queue[_active_shader_id].push({model_id, transform_id});
}


void
idk::RenderEngine::_draw_to_screen(GLuint texture_id)
{
    GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, 0); )
    GLCALL( glActiveTexture(GL_TEXTURE0); )
    GLCALL( glBindTexture(GL_TEXTURE_2D, texture_id); )
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
    for (auto &[shader_id, vec]: _model_draw_queue)
    {
        _gl_interface.bindShaderProgram(shader_id);
        // for (auto [model_id, transform_id]: vec)
        //     _draw_model(model_id, transform_id);
        vec.clear();
    }


    // Draw final texture to the screen quad
    _gl_interface.free_glTextureUnitIDs();
    _gl_interface.bindShaderProgram(_screenquad_shader);

    GLCALL( glDisable(GL_DEPTH_TEST); )
    GLCALL( glDisable(GL_CULL_FACE); )

    GLCALL( glBindVertexArray(_quad_VAO); )
    GLCALL( glDrawArrays(GL_TRIANGLES, 0, 6); )
    GLCALL( glBindVertexArray(0); )

    SDL_GL_SwapWindow(_SDL_window);
}

