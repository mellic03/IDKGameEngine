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
    GLuint gl_id;
    idk::__tex_file_t tex_file;
};


struct idk::Material
{
    glm::vec3   diffuse_color       = glm::vec3(0.0f);
    glm::vec3   specular_color      = glm::vec3(0.0f);
    float       specular_exponent   = 16.0f;
    GLuint      albedo_texture      = 0;
    GLuint      specular_texture    = 0;
    GLuint      normal_texture      = 0;
};


struct idk::Mesh
{
    int material_id = -1;
    GLuint IBO;
    std::vector<GLuint> vertex_indices;
};


struct idk::Model
{
    std::vector<idk::Vertex> vertices;
    std::vector<idk::Mesh> meshes;
    GLuint VAO, VBO;
};

