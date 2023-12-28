#include "idk_sdl_glew_init.hpp"
#include "idk_initflags.hpp"

#include <iostream>

using namespace idk::internal;


SDL_GLEW_Initializer::SDL_GLEW_Initializer( const char *windowname, int w, int h,
                                            uint8_t gl_version, uint32_t flags )
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
    if (flags & idk::InitFlag::INIT_HEADLESS) sdl_flags |= SDL_WINDOW_HIDDEN;

    SDL_window = SDL_CreateWindow(
        windowname,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        sdl_flags
    );

    int gl_major = idk::glVersionMajor(gl_version);
    int gl_minor = idk::glVersionMinor(gl_version);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor);


    SDL_GL_context = SDL_GL_CreateContext(SDL_window);
    SDL_GL_MakeCurrent(SDL_window, SDL_GL_context);
    SDL_GL_SetSwapInterval(0); // vsync
    SDL_SetRelativeMouseMode(SDL_FALSE);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error initializing glew\n";
        exit(1);
    }

    std::cout
        << "Initialized OpenGL version "
        << gl_major << "." << gl_minor
        << " context\n";

}
