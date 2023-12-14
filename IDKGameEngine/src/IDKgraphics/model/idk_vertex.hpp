#pragma once

#include <glm/glm.hpp>


namespace idk
{
    struct Vertex;
    struct AnimatedVertex;
};


struct idk::Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texcoords;
};


struct idk::AnimatedVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texcoords;

    glm::ivec4 bone_ids     = glm::ivec4(-1);
    glm::vec4  bone_weights = glm::vec4(0.0f);
};


