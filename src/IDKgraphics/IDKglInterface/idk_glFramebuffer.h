#pragma once

#include "IDKcommon/IDKcommon.h"

namespace idk { struct glFramebuffer; };


struct idk::glFramebuffer
{
    int width, height;
    GLuint FBO, RBO;
    idk::vector<GLuint> textures;
    glFramebuffer(size_t num_textures): textures(num_textures) {  };
};


