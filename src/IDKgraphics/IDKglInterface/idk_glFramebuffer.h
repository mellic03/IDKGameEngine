#pragma once

#include "IDKcommon/IDKcommon.h"

namespace idk { struct glFramebuffer; };


struct idk::glFramebuffer
{
    int width, height;
    GLuint FBO, RBO;
    std::vector<GLuint> output_textures;
};


