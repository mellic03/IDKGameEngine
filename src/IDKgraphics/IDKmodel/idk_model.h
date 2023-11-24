#pragma once

#include "IDKcommon/IDKcommon.h"
#include "idk_vertex.h"
#include "idk_filetypes.h"

#include <vector>


namespace idk
{
    struct Texture;
    struct Material;
    struct Mesh;
    struct Model;
};


struct idk::Texture
{
    GLuint gl_id = 0;
    idk::__tex_file_t tex_file;
};


struct idk::Material
{
    // glm::vec3   diffuse_color       = glm::vec3(0.0f);
    // glm::vec3   specular_color      = glm::vec3(0.0f);
    // float       specular_exponent   = 16.0f;
    // float       roughness           = 0.5f;

    GLuint      albedo_id        = 0;
    GLuint      metallic_id      = 0;
    GLuint      roughness_id     = 0;
    GLuint      ao_id            = 0;

    GLuint      normal_id        = 0;

    idk::__tex_file_t albedo_tex;
    idk::__tex_file_t specular_tex;
    idk::__tex_file_t normal_tex;

};


struct idk::Mesh
{
    int material_id;
    GLuint IBO;
    std::vector<GLuint> vertex_indices;
    Mesh(): material_id(0), IBO(0) {  };
};


struct idk::Model
{
    std::vector<idk::Vertex> vertices;
    std::vector<idk::Mesh> meshes;
    GLuint VAO, VBO;
};

