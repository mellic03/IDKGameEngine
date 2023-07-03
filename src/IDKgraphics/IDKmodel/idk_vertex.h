#pragma once

#include <glm/glm.hpp>


namespace idk
{
    struct Vertex;
};


struct idk::Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;
};

