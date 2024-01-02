#pragma once

#include <glm/glm.hpp>
#include <libidk/GL/common.hpp>


namespace idk
{
    enum VertexFormat: uint32_t
    {
        VERTEX_POSITION3F = 0,
        VERTEX_POSITION3F_UV2F,
        VERTEX_POSITION3F_NORMAL3F_TANGENT3F_UV2F,
        VERTEX_POSITION3F_NORMAL3F_TANGENT3F_UV2F_SKINNED,

        VERTEX_NUM_FORMATS
    };

    struct VertexFormatDescriptor
    {
        uint32_t stride;
        uint32_t attribs;

        uint32_t attrib_size[8];
        GLenum   attrib_datatype[8];
        uint32_t attrib_offset[8];
    };

    VertexFormatDescriptor getVertexFormatDescriptor( uint32_t format );

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
