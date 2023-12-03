#pragma once

#include "libidk/IDKgl.hpp"
#include "libidk/libidk.hpp"

namespace idk::noisegen3D
{
    GLuint worley( int w );
    GLuint worleyvec( int w );
    GLuint white( int w, int h, int d );
};


namespace idk::noisegen2D
{
    GLuint white( int w, int h );
};