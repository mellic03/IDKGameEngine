#include "renderer.h"

#include <iostream>


IDK::Renderer::Renderer(size_t w, size_t h)
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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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


    // Generate screen quad framebuffer
    // GLCALL( glGenFramebuffers(1, &_quad_FBO); )
    // GLCALL( glGenTextures(1, &_quad_texture); )
    // GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, _quad_FBO);)
    // GLCALL(glBindTexture(GL_TEXTURE_2D, _quad_texture);)
    // GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1000, 1000, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);)
    // GLCALL(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );)
    // GLCALL(glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );)
    // GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);)
    // GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);)
    // GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _quad_texture, 0);)
    // GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
    // GLCALL( glDrawBuffers(1, attachments); )
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    _screenquad_shader = Shader("assets/shaders/", "screenquad.vs", "screenquad.fs");
}


IDK::Renderer::~Renderer()
{
    SDL_DestroyWindow(_SDL_window);
}



void IDK::Renderer::_use_shader(Shader &shader)
{
    GLCALL( glUseProgram(shader.id()); )
}

void IDK::Renderer::useShader(IDK::Shader &shader)
{
    _active_shader_id = shader.id();
}
void IDK::Renderer::useShader(GLuint shader_id)
{
    _active_shader_id = shader_id;
}

void IDK::Renderer::addShader(Shader &shader)
{
    _shaders[shader.id()] = shader;
}


void IDK::Renderer::_draw_model(Model *model, IDK::transform *transform)
{
    // glBindVertexArray(model.vao);
    // ...
    // ...
    // ...
    // glBindVertexArray(0);
}

void IDK::Renderer::_draw_model(int id, IDK::transform *transform)
{

    // glBindVertexArray(model.vao);
    // ...
    // ...
    // ...
    // glBindVertexArray(0);
}


void IDK::Renderer::drawModel(Model &model, IDK::transform &transform)
{
    _model_queue[_active_shader_id].push({&model, &transform});
}

void IDK::Renderer::drawModel(int id, IDK::transform &transform)
{
    _model_id_queue[_active_shader_id].push({id, &transform});
}

void IDK::Renderer::_draw_to_screen(GLuint texture_id)
{
    GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, 0); )
    GLCALL( glActiveTexture(GL_TEXTURE0); )
    GLCALL( glBindTexture(GL_TEXTURE_2D, texture_id); )
}



void IDK::Renderer::beginFrame()
{
    GLCALL( glClearColor(0.0f, 0.0f, 0.0f, 1.0f); )
    GLCALL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); )
}


void IDK::Renderer::endFrame()
{
    for (auto &[shader_id, mt_pairs]: _model_queue)
    {
        _use_shader(_shaders[shader_id]);

        for (auto &[modelptr, tptr]: mt_pairs)
            _draw_model(modelptr, tptr);

        mt_pairs.clear();
    }

    for (auto &[shader_id, it_pairs]: _model_id_queue)
    {
        _use_shader(_shaders[shader_id]);

        for (auto &[model_id, tptr]: it_pairs)
            _draw_model(model_id, tptr);

        it_pairs.clear();
    }


    _use_shader(_screenquad_shader);

    GLCALL( glDisable(GL_DEPTH_TEST); )
    GLCALL( glDisable(GL_CULL_FACE); )

    GLCALL( glBindVertexArray(_quad_VAO); )
    GLCALL( glDrawArrays(GL_TRIANGLES, 0, 6); )
    GLCALL( glBindVertexArray(0); )

    SDL_GL_SwapWindow(_SDL_window);
}
