#pragma once

#include "IDKcore.h"

#include <string>
#include <fstream>
#include <sstream>


struct idk::vertex
{
    glm::vec3 position  = glm::vec3(0.0f);
    glm::vec3 normal    = glm::vec3(0.0f);
    glm::vec2 texcoords = glm::vec2(0.0f);
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


class idk::Model
{
private:
    std::unordered_map<std::string, GLuint> _material_ids;
    void                        _load_obj( std::string src );
    void                        _load_mtl( std::string src, Allocator<Material> &materials,
                                           std::unordered_map<std::string, GLuint> &textures );
    void                        _gen_mesh_IBO( size_t mesh_idx );

public:
    int                         id;
    std::vector<GLuint>         IBOS;
    std::vector<Mesh>           meshes; // One mesh per material.
    std::vector<idk::vertex>    vertex_data;

                                Model() {  };
                                Model( std::string root, std::string obj, std::string mtl,
                                       Allocator<Material> &materials,
                                       std::unordered_map<std::string, GLuint> &textures  );

                                Model( std::string obj_src, std::string mtl_src,
                                       Allocator<Material> &materials,
                                       std::unordered_map<std::string, GLuint> &textures  );
};
