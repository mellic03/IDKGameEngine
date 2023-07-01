#pragma once

#include "../IDKgraphics_common.h"

#include "IDK_vertex.h"
#include "IDK_texture.h"



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


namespace idk_filetools
{
    idk::__vts_file_t new_vts(uint32_t size);
    idk::__vts_file_t new_vts(uint32_t size, idk::Vertex *data);

    void vts_to_bin( idk::__vts_file_t &, std::string filepath );
    idk::__vts_file_t vts_from_bin( std::string filepath );
};


