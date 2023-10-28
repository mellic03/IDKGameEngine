#pragma once

#include "IDKcommon/IDKcommon.h"

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