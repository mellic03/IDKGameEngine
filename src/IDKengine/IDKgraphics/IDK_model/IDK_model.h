#pragma once

#include "../IDKgraphics_common.h"

#include "IDK_filetypes.h"

#include <string>


namespace idk
{
    void    load_texture( std::string filepath, idk::Texture & );
};


struct idk::Texture
{
    GLuint id;
    uint32_t w, h;
    uint8_t *data;
};


struct idk::Material
{
    glm::vec3   diffuse_color       = glm::vec3(0.0f);
    glm::vec3   specular_color      = glm::vec3(0.0f);
    float       specular_exponent   = 16.0f;
    GLuint      albedo_texture;
    GLuint      specular_texture;
    GLuint      normal_texture;
};


struct idk::Mesh
{
    uint                    material_id;
    std::vector<GLuint>     vertex_indices;
    GLuint                  VAO, VBO;
};


struct idk::Model
{
private:
    std::unordered_map<std::string, GLuint> _material_ids;
    void                        _load_obj( std::string src );
    void                        _load_mtl( std::string src, Allocator<Material> &materials,
                                           std::unordered_map<std::string, GLuint> &textures );
    void                        _gen_mesh_IBO( size_t mesh_idx );

public:

    std::vector<idk::Vertex> new_vertices;
    std::vector<GLuint> new_IBOs;
    std::vector<idk::Mesh> new_meshes;

    int                         id;
    std::vector<GLuint>         IBOS;
    std::vector<Mesh>           meshes; // One mesh per material.
    std::vector<idk::Vertex>    vertex_data;

                                Model() {  };
                                Model( std::string root, std::string obj, std::string mtl,
                                       Allocator<Material> &materials,
                                       std::unordered_map<std::string, GLuint> &textures  );

                                Model( std::string obj_src, std::string mtl_src,
                                       Allocator<Material> &materials,
                                       std::unordered_map<std::string, GLuint> &textures  );
};
