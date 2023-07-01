#include "IDK_tools.h"

#include <fstream>
#include <sstream>

void
idktools::vertices_to_bin( std::vector<idk::Vertex> &vertices, std::string filepath )
{
    std::ofstream stream(filepath, std::ios_base::binary);
    
    // Write number of vertices to file
    int num_vertices = vertices.size();
    stream.write(
        reinterpret_cast<const char *>(&num_vertices),
        sizeof(num_vertices)
    );

    // Write vertices to file
    stream.write(
        reinterpret_cast<const char *>(&vertices[0]),
        num_vertices * sizeof(idk::Vertex)
    );

    stream.close();
};


std::vector<idk::Vertex>
idktools::vertices_from_bin( std::string filepath )
{
    std::ifstream stream(filepath, std::ios_base::binary);

    // read number of vertices from file
    int num_vertices;
    stream.read(
        reinterpret_cast<char *>(&num_vertices),
        sizeof(num_vertices)
    );

    std::vector<idk::Vertex> vertices(num_vertices);

    // read vertices from file
    stream.read(
        reinterpret_cast<char *>(&vertices[0]),
        num_vertices * sizeof(idk::Vertex)
    );

    stream.close();
    return vertices;
};


