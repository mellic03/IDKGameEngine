#pragma once

#include "../IDKcore/IDKcore.h"
#include <string>
#include <fstream>
#include <sstream>


struct vertex
{
    IDK::vec3 position  = IDK::vec3(0.0f);
    IDK::vec3 normal    = IDK::vec3(0.0f);
    IDK::vec2 texcoords = IDK::vec2(0.0f);
    IDK::vec3 tangent   = IDK::vec3(0.0f);
};


class IDK::Texture
{
private:
    int _id;

public:
    

};


class IDK::Material
{
private:
    IDK::vec3 _diffuse_color;
    IDK::vec3 _specular_color;
    float _specular_exponent;
    int _texture_id;

public:

};


class Mesh
{
private:
    IDK::vector<GLuint> _vertex_indices;
    int _material_id;

public:
    Mesh() {  };

};


class IDK::Model
{
private:
    IDK::vector<vertex> _vertices;
    IDK::vector<Mesh> _meshes;

    void _load_obj(std::string &path);
    void _load_mtl(std::string &path);

public:
    Model() {  };
    Model(std::string dir, std::string obj, std::string mtl) {  };

};
