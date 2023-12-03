#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#include "idk_vertex.hpp"
#include "libidk/libidk.hpp"



namespace idk
{
    struct PODvertex;
    struct internal_mesh_t;
    struct model_file_t;
};



struct idk::PODvertex
{
    float position[3];
    float normal[3];
    float tangent[3];
    float texcoords[2];
};



// struct idk::internal_mesh_t
// {
//     int albedo, metallic, roughness;
//     int normal, ambient,  displacement;

//     idk::PODvertex *vertices;
// };



// idk::internal_mesh_t new_mesh_t()
// {
//     idk::internal_mesh_t mesh = {
//         -1, -1, -1,
//         -1, -1, -1,
//         nullptr
//     };

//     return mesh;
// }



// num_textures   num_meshes   sizeof(mesh_1)   sizeof(mesh_2)   verts_1   verts_2
// 1              2            32               64               xyzxyzuv  xyzxyzuv


// struct idk::model_file_t
// {
//     std::vector<std::string>     texture_paths;
//     std::vector<internal_mesh_t> meshes;
// };


