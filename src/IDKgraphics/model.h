#pragma once

#include "IDKgraphics_common/IDKgraphics_common.h"
#include <string>
#include <fstream>
#include <sstream>


struct idk::vertex
{
    glm::vec3 position  = glm::vec3(0.0f);
    glm::vec3 normal    = glm::vec3(0.0f);
    glm::vec2 texcoords = glm::vec2(0.0f);
};



class idk::Material
{
private:
public:
    glm::vec3   diffuse_color;
    glm::vec3   specular_color;
    float       specular_exponent;
    GLuint      albedo_texture;
    GLuint      specular_texture;
    GLuint      normal_texture;

};


class idk::Mesh
{
public:
    uint                    material_id;
    std::vector<GLuint>     vertex_indices;
    GLuint                  VAO, VBO;

};


class idk::Model
{
private:
    std::unordered_map<std::string, GLuint> _material_ids;
    std::vector<idk::vertex>    _vertices;
    void                        _load_obj( std::string path );
    void                        _load_mtl( std::string path, Allocator<Material> &materials,
                                           std::unordered_map<std::string, GLuint> &textures );
    void                        _gen_mesh_IBO( size_t mesh_idx );

public:
    int                         id;
    std::vector<GLuint>         IBOS;
    std::vector<Mesh>           meshes; // One mesh per material.

                                Model() {  };
                                Model( std::string root, std::string obj, std::string mtl,
                                       Allocator<Material> &materials,
                                       std::unordered_map<std::string, GLuint> &textures  );

};
