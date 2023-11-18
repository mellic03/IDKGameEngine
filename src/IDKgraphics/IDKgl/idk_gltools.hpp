#pragma once

#include <string>
#include "IDKcommon/IDKcommon.h"

#include "idk_glFramebuffer.hpp"
#include "idk_glBindings.hpp"


namespace idk::gltools
{
    GLuint  loadTexture( size_t w, size_t h, uint32_t *data, bool srgb = false );
    GLuint  loadTexture( std::string filepath, bool srgb = false );

};

