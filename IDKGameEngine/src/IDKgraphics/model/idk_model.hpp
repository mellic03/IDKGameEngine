#pragma once

#include "libidk/IDKgl.hpp"
#include "libidk/libidk.hpp"
#include "idk_vertex.hpp"
#include "idk_filetypes.h"

#include <vector>


namespace idk
{
    struct Material;
    struct Mesh;

    struct BaseModel;
    struct Model;
    struct AnimatedModel;
};



struct idk::Material
{
    // enum Reflectance: int { NONMETAL, IRON, COPPER, GOLD, ALUMINIUM };

    // static std::string reflectance_str( int reflectance )
    // {
    //     switch (reflectance)
    //     {
    //         default:        return "???";
    //         case NONMETAL:  return "NONMETAL";
    //         case IRON:      return "IRON";
    //         case COPPER:    return "COPPER";
    //         case GOLD:      return "GOLD";
    //         case ALUMINIUM: return "ALIMINIUM";
    //     }
    // }

    std::string name = "";

    GLuint      albedo_id        = 0;
    GLuint      rm_id            = 0;
    GLuint      ao_id            = 0;
    GLuint      displacement_id  = 0;
    GLuint      normal_id        = 0;

    float       metallic_strength     = 1.0f;
    float       roughness_strength    = 1.0f;
    float       displacement_strength = 0.1f;
    float       normal_strength       = 1.0f;

    glm::vec3   reflectance = glm::vec3(0.04f);
};


struct idk::Mesh
{
    int material_id;
    GLuint num_indices;
};


struct idk::BaseModel
{
    std::vector<idk::Mesh>   meshes;
    idk::Buffer<uint32_t>    m_indices;

    GLuint VAO, VBO, IBO;

    virtual idk::iBuffer *vertices() = 0;
    virtual idk::iBuffer *indices() { return &m_indices; };
};



struct idk::Model: public BaseModel
{
    idk::Buffer<idk::Vertex>  m_vertices;

    virtual idk::iBuffer *vertices() final { return &m_vertices; };
};


constexpr int IDK_ANIMATED_BIT = (int)(1 << (8*sizeof(int) - 2));

struct idk::AnimatedModel
{
    idk::Buffer<idk::AnimatedVertex>  m_vertices;

    virtual idk::iBuffer *vertices() final { return &m_vertices; };
};

