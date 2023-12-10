#pragma once

#include "idk_filetypes.h"
#include "idk_animation.hpp"
#include <libidk/libidk.hpp>
#include <libidk/IDKgl.hpp>

namespace idk::filetools
{
    idk::idkvi_header_t readheader( const std::string & );

    // void readidkvi( const idkvi_header_t &header, const std::string &filepath,
    //                 idk::iBuffer *vertices, idk::iBuffer *indices );

    // void readidkvi( const idkvi_header_t &header, const std::string &filepath,
    //                 std::unique_ptr<void> &vertices, std::vector<uint32_t> &indices );

    void readidka( const idkvi_header_t &header, const std::string &filepath,
                   std::vector<idk::Animation> & );

    template <typename vertex_t>
    void readidkvi( const idkvi_header_t &header, const std::string &filepath,
                       std::vector<vertex_t> &vertices, std::vector<uint32_t> &indices );


};


template <typename vertex_t>
void idk::filetools::readidkvi( const idkvi_header_t &header, const std::string &filepath,
                                   std::vector<vertex_t> &vertices, std::vector<uint32_t> &indices )
{
    std::ifstream stream(filepath, std::ios::binary);

    size_t num_vertices, num_indices;
    stream.read(reinterpret_cast<char *>(&num_vertices), sizeof(size_t));
    stream.read(reinterpret_cast<char *>(&num_indices),  sizeof(size_t));

    vertices.resize(num_vertices);
    indices.resize(num_indices);

    stream.read(reinterpret_cast<char *>(vertices.data()), num_vertices * sizeof(vertex_t));
    stream.read(reinterpret_cast<char *>(indices.data()),  num_indices * sizeof(uint32_t));

    stream.close();
}

