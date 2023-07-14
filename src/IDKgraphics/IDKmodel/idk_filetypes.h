#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#include "idk_vertex.h"


namespace idk
{
    struct __tex_file_t; // texture
    struct __mat_file_t; // material
    struct __idx_file_t; // indices
    struct __vts_file_t; // vertices
    struct __mdl_file_t; // model
};


/** Binary texture file
*/
struct idk::__tex_file_t
{
    int w, h;       // width*height == number of uint8_t, not pixels
    uint32_t *data;
};


struct idk::__mat_file_t
{
    float       diff[3];
    float       spec[3];
    float       spec_exp;

    idk::__tex_file_t diffuse_tex;
    idk::__tex_file_t specular_tex;
    idk::__tex_file_t normal_tex;
};


struct idk::__vts_file_t
{
    int size; // number of Vertices
    idk::Vertex *data;
};


struct idk::__mdl_file_t
{
    std::vector<std::string> tex_filepaths;
    std::vector<std::string> vts_filepaths;
};
