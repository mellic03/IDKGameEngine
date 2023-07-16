#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#include "idk_vertex.h"
#include "IDKcommon/IDKcommon.h"

namespace idk
{
    struct __tex_file_t;    // texture
    struct __texpak_file_t; // package of textures

    struct __mat_file_t; // material
    struct __idx_file_t; // indices
    struct __vts_file_t; // vertices
    struct __mdl_file_t; // model
};


struct idk::__tex_file_t
{
    char *name;
    size_t w, h;
    uint32_t *data;
};


struct idk::__texpak_file_t
{
    idk::vector<idk::__tex_file_t>  texfiles;
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
    size_t size; // number of Vertices
    idk::Vertex *data;
};


struct idk::__mdl_file_t
{
    std::vector<std::string> tex_filepaths;
    std::vector<std::string> vts_filepaths;
};
