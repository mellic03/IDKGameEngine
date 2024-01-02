#pragma once

#include <cstdint>


namespace idk
{
    enum TextureIndex { ALBEDO_IDX, NORMAL_IDX, RM_IDX, AO_IDX, NUM_IDX };
    enum TextureBit   { ALBEDO_BIT=1<<0, NORMAL_BIT=1<<1, RM_BIT=1<<2, AO_BIT=1<<3 };

    struct idkvi_header_t;
    struct idkvi_mesh_t;
    struct idkvi_file_t;

    void idkvi_load  ( const char *path, idkvi_file_t & );
    void idkvi_write ( const char *path, idkvi_file_t & );
    void idkvi_free  ( idkvi_file_t & );
};


struct idk::idkvi_mesh_t
{
    uint32_t    indices_offset;
    uint32_t    indices_size;
};


struct idk::idkvi_header_t
{
    uint32_t   version_major;
    uint32_t   version_minor;

    uint32_t   sizeof_vertex;
    uint32_t   num_animations;

    uint32_t   num_meshes;
    uint32_t   num_indices;
    uint32_t   num_vertices;
};


struct idk::idkvi_file_t
{
    idkvi_header_t header;

    void          *vertices;
    uint32_t      *indices;

    idkvi_mesh_t  *meshes;
};

