#pragma once

#include <cstdint>
#include <string>

#include "idk_vertex.h"


namespace idk
{
    struct __tex_file_t;
    struct __vts_file_t;
    struct __mdl_file_t;
};


namespace idk::filetools
{
    void    tex_save(std::string filepath, idk::__tex_file_t &);
    void    tex_load(std::string filepath, idk::__tex_file_t &);
    
    void    vts_save(std::string filepath, idk::__vts_file_t &);
    void    vts_load(std::string filepath, idk::__vts_file_t &);

    void    mdl_save(std::string filepath, idk::__mdl_file_t &);
    void    mdl_load(std::string filepath, idk::__mdl_file_t &);
};


struct idk::__tex_file_t
{
    int w, h; // width*height == number of uint8_t, not pixels
    uint8_t *data;
};


struct idk::__vts_file_t
{
    int size; // number of Vertices
    idk::Vertex *data;
};


struct idk::__mdl_file_t
{

};



