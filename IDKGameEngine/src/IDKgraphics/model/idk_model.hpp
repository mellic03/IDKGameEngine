#pragma once

#include "libidk/IDKgl.hpp"
#include "libidk/libidk.hpp"
#include "idk_vertex.hpp"
#include "idk_filetypes.h"
#include "idk_animation.hpp"

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
    GLuint      rm_id            = 0;
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
    bool animated = false;
    std::vector<idk::Animation> m_animations;

    std::vector<idk::Mesh>    meshes;
    std::vector<uint32_t>     m_indices;
    std::vector<idk::Vertex>  m_vertices;
    std::vector<idk::AnimatedVertex> m_anim_vertices;

    GLuint VAO, VBO, IBO;

};

