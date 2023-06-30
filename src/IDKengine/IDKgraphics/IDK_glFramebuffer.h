#pragma once

#include "IDKcore.h"


struct idk::glFramebuffer
{
    GLuint FBO, RBO;
    idk::vector<GLuint> textures;
    glFramebuffer(size_t num_textures): textures(num_textures) {  };
};

