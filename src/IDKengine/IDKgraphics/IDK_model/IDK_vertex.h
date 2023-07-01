#pragma once

#include "../IDKgraphics_common.h"


struct idk::Vertex
{
    glm::vec3 position  = glm::vec3(0.0f);
    glm::vec3 normal    = glm::vec3(0.0f);
    glm::vec2 texcoords = glm::vec2(0.0f);
};


