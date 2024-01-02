#include "idk_vertex.hpp"
#include <array>


// VERTEX_POSITION3F,
// VERTEX_POSITION3F_UV2F,
// VERTEX_POSITION3F_NORMAL3F_TANGENT3F_UV2F,
// VERTEX_POSITION3F_NORMAL3F_TANGENT3F_UV2F_SKINNED,

struct Vertex_POSITION3F
{
    glm::vec3 position;
};

struct Vertex_POSITION3F_UV2F
{
    glm::vec3 position;
    glm::vec2 texcoords;
};

struct Vertex_POSITION3F_NORMAL3F_TANGENT3F_UV2F
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texcoords;
};

struct Vertex_POSITION3F_NORMAL3F_TANGENT3F_UV2F_SKINNED
{
    glm::vec3  position;
    glm::vec3  normal;
    glm::vec3  tangent;
    glm::vec2  texcoords;
    glm::ivec4 boneids;
    glm::vec4  weights;
};


static Vertex_POSITION3F A;
static Vertex_POSITION3F_UV2F B;
static Vertex_POSITION3F_NORMAL3F_TANGENT3F_UV2F C;
static Vertex_POSITION3F_NORMAL3F_TANGENT3F_UV2F_SKINNED D;


idk::VertexFormatDescriptor
get_descriptor(Vertex_POSITION3F V)
{
    idk::VertexFormatDescriptor desc;

    desc.stride  = sizeof(Vertex_POSITION3F);
    desc.attribs = 2;

    desc.attrib_size[0] = sizeof(V.position);

    desc.attrib_datatype[0] = GL_FLOAT;

    size_t offset = 0;
    desc.attrib_offset[0] = offset;  offset += desc.attrib_size[0];

    return desc;
}


idk::VertexFormatDescriptor
get_descriptor(Vertex_POSITION3F_UV2F V)
{
    idk::VertexFormatDescriptor desc;

    desc.stride  = sizeof(Vertex_POSITION3F_UV2F);
    desc.attribs = 2;

    desc.attrib_size[0] = sizeof(V.position);
    desc.attrib_size[1] = sizeof(V.texcoords);

    desc.attrib_datatype[0] = GL_FLOAT;
    desc.attrib_datatype[1] = GL_FLOAT;

    size_t offset = 0;
    desc.attrib_offset[0] = offset;  offset += desc.attrib_size[0];
    desc.attrib_offset[1] = offset;  offset += desc.attrib_size[1];

    return desc;
}


idk::VertexFormatDescriptor
get_descriptor(Vertex_POSITION3F_NORMAL3F_TANGENT3F_UV2F V)
{
    idk::VertexFormatDescriptor desc;

    desc.stride  = sizeof(Vertex_POSITION3F_NORMAL3F_TANGENT3F_UV2F);
    desc.attribs = 2;

    int idx;
    size_t offset;

    idx = 0;
    desc.attrib_size[idx] = sizeof(V.position);   idx += 1;
    desc.attrib_size[idx] = sizeof(V.normal);     idx += 1;
    desc.attrib_size[idx] = sizeof(V.tangent);    idx += 1;
    desc.attrib_size[idx] = sizeof(V.texcoords);  idx += 1;

    idx = 0;
    desc.attrib_datatype[idx] = GL_FLOAT;   idx += 1;
    desc.attrib_datatype[idx] = GL_FLOAT;   idx += 1;
    desc.attrib_datatype[idx] = GL_FLOAT;   idx += 1;
    desc.attrib_datatype[idx] = GL_FLOAT;   idx += 1;


    idx = 0;
    offset = 0;

    for (int i=0; i<4; i++)
    {
        desc.attrib_offset[i] = offset;
        offset += desc.attrib_size[i];
    }

    return desc;
}


idk::VertexFormatDescriptor
get_descriptor(Vertex_POSITION3F_NORMAL3F_TANGENT3F_UV2F_SKINNED V)
{
    idk::VertexFormatDescriptor desc;

    desc.stride  = sizeof(Vertex_POSITION3F_NORMAL3F_TANGENT3F_UV2F);
    desc.attribs = 2;

    int idx;
    size_t offset;

    idx = 0;
    desc.attrib_size[idx] = sizeof(V.position);   idx += 1;
    desc.attrib_size[idx] = sizeof(V.normal);     idx += 1;
    desc.attrib_size[idx] = sizeof(V.tangent);    idx += 1;
    desc.attrib_size[idx] = sizeof(V.texcoords);  idx += 1;
    desc.attrib_size[idx] = sizeof(V.boneids);    idx += 1;
    desc.attrib_size[idx] = sizeof(V.weights);    idx += 1;

    idx = 0;
    desc.attrib_datatype[idx] = GL_FLOAT;   idx += 1;
    desc.attrib_datatype[idx] = GL_FLOAT;   idx += 1;
    desc.attrib_datatype[idx] = GL_FLOAT;   idx += 1;
    desc.attrib_datatype[idx] = GL_FLOAT;   idx += 1;
    desc.attrib_datatype[idx] = GL_INT;     idx += 1;
    desc.attrib_datatype[idx] = GL_FLOAT;   idx += 1;


    idx = 0;
    offset = 0;

    for (int i=0; i<6; i++)
    {
        desc.attrib_offset[i] = offset;
        offset += desc.attrib_size[i];
    }

    return desc;
}


idk::VertexFormatDescriptor
idk::getVertexFormatDescriptor( uint32_t format )
{
    switch (format)
    {
        default:
        case VERTEX_POSITION3F:
            return get_descriptor(A);
            break;

        case VERTEX_POSITION3F_UV2F:
            return get_descriptor(B);
            break;

        case VERTEX_POSITION3F_NORMAL3F_TANGENT3F_UV2F:
            return get_descriptor(C);
            break;

        case VERTEX_POSITION3F_NORMAL3F_TANGENT3F_UV2F_SKINNED:
            return get_descriptor(D);
            break;
    }
}
