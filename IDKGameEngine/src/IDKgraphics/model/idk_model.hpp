#pragma once

#include <libidk/IDKgl.hpp>
#include <libidk/libidk.hpp>
#include "idk_vertex.hpp"

#include <vector>


namespace idk
{
    struct Material;
    struct Mesh;

    struct Model;
};



struct idk::Material
{
    std::string name = "";

    GLuint      albedo_id        = 0;
    GLuint      arm_id           = 0;
    GLuint      ao_id            = 0;
    GLuint      displacement_id  = 0;
    GLuint      normal_id        = 0;

    float       metallic_strength     = 1.0f;
    float       roughness_strength    = 1.0f;
    float       displacement_strength = 0.1f;
    float       normal_strength       = 1.0f;
};


struct idk::Mesh
{
    int material_id;
    GLuint num_indices;
};


struct idk::Model
{
    float alpha = 0.0f;

    idk::iBuffer *m_buffer;

    std::vector<idk::Mesh>    meshes;
    std::vector<idk::Vertex>  m_vertices;
    std::vector<uint32_t>     m_indices;

    // Animation
    // ---------------------------------------------------
    bool animated = false;
    int  animator_id = -1;
    std::vector<idk::AnimatedVertex> m_anim_vertices;
    // ---------------------------------------------------

    GLuint VAO, VBO, IBO;
};

