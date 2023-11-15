#pragma once

#include <string>
#include "IDKcommon/IDKcommon.h"

#include "idk_glFramebuffer.hpp"
#include "idk_glBindings.hpp"


namespace idk::gltools
{
    GLuint  loadTexture( int w, int h, uint32_t *data, bool srgb = false );

};

