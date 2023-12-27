#pragma once

#include <glm/glm.hpp>
#include <vector>


namespace idk
{
    struct VertexElement;
    struct VertexDescriptor;
};


struct idk::VertexElement
{
    uint32_t sizeof_element;
    uint32_t offsetof_element;
};


struct idk::VertexDescriptor
{
    std::vector<idk::VertexElement> elements;
};
