#include "idk_idkvi.hpp"


#include <stdio.h>
#include <stdlib.h>


void
idk::idkvi_load( const char *path, idkvi_file_t &file )
{
    FILE *fh = fopen(path, "r");

    idkvi_header_t &header = file.header;
    void *data;

    data = reinterpret_cast<void *>(&header);
    fread(data, sizeof(uint32_t), sizeof(idkvi_header_t)/sizeof(uint32_t), fh);

    file.vertices = (void *)malloc(header.num_vertices * header.sizeof_vertex);
    file.indices  = (uint32_t *)malloc(header.num_indices * sizeof(uint32_t));
    file.meshes   = (idkvi_mesh_t *)malloc(header.num_meshes * sizeof(idkvi_mesh_t));

    data = file.vertices;
    fread(data, header.sizeof_vertex, header.num_vertices, fh);

    data = reinterpret_cast<void *>(file.indices);
    fread(data, sizeof(uint32_t), header.num_indices, fh);

    data = reinterpret_cast<void *>(file.meshes);
    fread(data, sizeof(idkvi_mesh_t), header.num_meshes, fh);

    fclose(fh);
}


void
idk::idkvi_write( const char *path, idkvi_file_t &file )
{
    FILE *fh = fopen(path, "w");

    idkvi_header_t &header = file.header;
    void *data;

    data = reinterpret_cast<void *>(&header);
    fwrite(data, sizeof(uint32_t), sizeof(idkvi_header_t)/sizeof(uint32_t), fh);

    data = file.vertices;
    fwrite(data, header.sizeof_vertex, header.num_vertices, fh);

    data = reinterpret_cast<void *>(file.indices);
    fwrite(data, sizeof(uint32_t), header.num_indices, fh);

    data = reinterpret_cast<void *>(file.meshes);
    fwrite(data, sizeof(idkvi_mesh_t), header.num_meshes, fh);

    fclose(fh);
}


void
idk::idkvi_free( idkvi_file_t &file )
{
    free(file.vertices);
    free(file.indices);
    free(file.meshes);
}
