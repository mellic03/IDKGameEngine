#pragma once

#include <libidk/IDKSDL2.hpp>

namespace idk::internal { class SDL_GLEW_Initializer; };

class idk::internal::SDL_GLEW_Initializer
{
public:
    SDL_Window *    SDL_window;
    SDL_GLContext   SDL_GL_context;

    SDL_GLEW_Initializer( const char *windowname, int w, int h, int gl_major,
                          int gl_minor, uint32_t flags );

};
