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
    glm::vec3 tangent = glm::vec3(0.0f);
    glm::vec2 texcoords;
};

