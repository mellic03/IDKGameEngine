#pragma once

#include "IDKcommon/IDKcommon.h"

namespace idk { struct glFramebuffer; };


struct idk::glFramebuffer
{
    int width  = 0;
    int height = 0;
    GLuint FBO = 0;
    GLuint RBO = 0;
    std::vector<GLuint> output_textures;
};


