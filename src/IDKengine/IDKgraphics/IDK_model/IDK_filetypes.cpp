#include "IDK_filetypes.h"

#include <fstream>


idk::__vts_file_t idk_filetools::new_vts(uint32_t size)
{
    idk::__vts_file_t vts;
    vts.size = size;
    vts.data = new idk::Vertex[size];
    return vts;
}


idk::__vts_file_t idk_filetools::new_vts(uint32_t size, idk::Vertex *data)
{
    idk::__vts_file_t vts;
    vts.size = size;
    vts.data = data;
    return vts;
}


void
idk_filetools::vts_to_bin( idk::__vts_file_t &vts, std::string filepath )
{
    std::ofstream stream(filepath, std::ios_base::binary);
    
    // Write number of vertices to file
    int num_vertices = vts.size;
    stream.write(
        reinterpret_cast<const char *>(&num_vertices),
        sizeof(num_vertices)
    );

    // Write vertices to file
    stream.write(
        reinterpret_cast<const char *>(vts.data),
        num_vertices * sizeof(idk::Vertex)
    );

    stream.close();
}


idk::__vts_file_t
idk_filetools::vts_from_bin( std::string filepath )
{
    std::ifstream stream(filepath, std::ios_base::binary);

    // read number of vertices from file
    int num_vertices;
    stream.read(
        reinterpret_cast<char *>(&num_vertices),
        sizeof(num_vertices)
    );

    std::vector<idk::Vertex> vertices(num_vertices);

    idk::__vts_file_t vts;
    vts.data = new idk::Vertex[num_vertices];

    // read vertices from file
    stream.read(
        reinterpret_cast<char *>(vts.data),
        num_vertices * sizeof(idk::Vertex)
    );

    stream.close();

    return vts;
}


