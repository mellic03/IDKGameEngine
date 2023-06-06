#pragma once

#include "IDKgraphics_common/IDKgraphics_common.h"
#include <string>
#include <fstream>
#include <sstream>


struct idk::vertex
{
    glm::vec4 position  = glm::vec4(0.0f);
    glm::vec4 normal    = glm::vec4(0.0f);
    glm::vec4 tangent   = glm::vec4(0.0f);
    glm::vec2 texcoords = glm::vec2(0.0f);
};



class idk::Material
{
private:
public:
    int         id;
    glm::vec3   diffuse_color;
    glm::vec3   specular_color;
    float       specular_exponent;
    int         texture_id;

};


class idk::Mesh
{
private:

public:
    int                     id;
    idk::vector<GLuint>     vertex_indices;
    GLuint                  VAO, VBO;

};


class idk::Model
{
private:
    idk::vector<idk::vertex>    _vertices;
    void                        _load_obj(std::string path);
    void                        _load_mtl(std::string path);
    void                        _gen_mesh_IBO(size_t mesh_idx);

public:
    int                         id;
    idk::vector<GLuint>         IBOS;
    idk::vector<Mesh>           meshes; // One mesh per material.

                                Model() {  };
                                Model(std::string root, std::string obj, std::string mtl);

};
