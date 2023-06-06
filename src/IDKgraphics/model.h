#pragma once

#include "../IDKcore/IDKcore.h"
#include <string>
#include <fstream>
#include <sstream>


struct idk::vertex
{
    idk::vec4 position  = idk::vec4(0.0f);
    idk::vec4 normal    = idk::vec4(0.0f);
    idk::vec4 tangent   = idk::vec4(0.0f);
    idk::vec2 texcoords = idk::vec2(0.0f);
};



class idk::Material
{
public:
    idk::vec3 diffuse_color;
    idk::vec3 specular_color;
    float specular_exponent;
    int texture_id;

};


class idk::Mesh
{
private:
    idk::vector<GLuint> _vertex_indices;

public:
    int material_id;
    idk::vector<GLuint> IBOS;

};


class idk::Model
{
private:
    idk::vector<idk::vertex> _vertices;

    void _load_obj(std::string path);
    void _load_mtl(std::string path);

public:
    Model() {  };
    Model(std::string root, std::string obj, std::string mtl);

    GLuint VAO;
    idk::vector<Mesh> meshes;

};
