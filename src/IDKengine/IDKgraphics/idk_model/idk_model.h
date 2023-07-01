#pragma once

#include "idk_filetools.h"

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
    GLuint id;
    uint32_t w, h;
    uint8_t *data;
};


struct idk::Material
{

};


struct idk::Mesh
{
    int material_id;
    std::vector<int> vertex_indices;
};


class idk::Model
{

};

