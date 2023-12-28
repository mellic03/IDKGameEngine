#pragma once

#include <libidk/IDKgl/common.hpp>


namespace idk::internal { class SDL_GLEW_Initializer; };

class idk::internal::SDL_GLEW_Initializer
{
public:
    SDL_Window *    SDL_window;
    SDL_GLContext   SDL_GL_context;

    SDL_GLEW_Initializer( const char *windowname, int w, int h, uint8_t gl_version,
                          uint32_t flags );

};
