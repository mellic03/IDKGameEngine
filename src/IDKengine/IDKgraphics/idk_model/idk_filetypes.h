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



struct idk::__tex_file_t
{
    uint32_t size;
    uint8_t *data;
};


struct idk::__vts_file_t
{
    uint32_t size;
    idk::Vertex *data;
};


struct idk::__mdl_file_t
{

};


namespace idk::filetools
{
    void    savetex(std::string filepath, idk::__tex_file_t &);
    void    loadtex(std::string filepath, idk::__tex_file_t &);
    
    void    savevts(std::string filepath, idk::__vts_file_t &);
    void    loadvts(std::string filepath, idk::__vts_file_t &);

    void    savemdl(std::string filepath, idk::__mdl_file_t &);
    void    loadmdl(std::string filepath, idk::__mdl_file_t &);
};

