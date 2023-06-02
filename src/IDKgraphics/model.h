#pragma once

#include "../IDKcore/IDKcore.h"
#include <string>
#include <fstream>
#include <sstream>


struct IDK::vertex
{
    IDK::vec3 position  = IDK::vec3(0.0f);
    IDK::vec3 normal    = IDK::vec3(0.0f);
    IDK::vec2 texcoords = IDK::vec2(0.0f);
    IDK::vec3 tangent   = IDK::vec3(0.0f);
};



class IDK::Material
{
public:
    IDK::vec3 diffuse_color;
    IDK::vec3 specular_color;
    float specular_exponent;
    int texture_id;

};


class IDK::Mesh
{
private:
    IDK::vector<GLuint> _vertex_indices;

public:
    int material_id;
    IDK::vector<GLuint> IBOS;

};


class IDK::Model
{
private:
    IDK::vector<IDK::vertex> _vertices;

    void _load_obj(std::string &path);
    void _load_mtl(std::string &path);

public:
    Model() {  };
    Model(std::string dir, std::string obj, std::string mtl) {  };

    GLuint VAO;
    IDK::vector<Mesh> meshes;

};
