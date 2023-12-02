#pragma once

#include "libidk/libidk.h"
#include "idk_vertex.h"
#include "idk_filetypes.h"

#include <vector>


namespace idk
{
    struct Texture;
    struct Material;
    struct Mesh;
    struct Model;
};


struct idk::Texture
{
    GLuint gl_id = 0;
    idk::__tex_file_t tex_file;
};



struct idk::Material
{
    enum Reflectance: int { NONMETAL, IRON, COPPER, GOLD, ALUMINIUM };

    static std::string reflectance_str( int reflectance )
    {
        switch (reflectance)
        {
            default:        return "???";
            case NONMETAL:  return "NONMETAL";
            case IRON:      return "IRON";
            case COPPER:    return "COPPER";
            case GOLD:      return "GOLD";
            case ALUMINIUM: return "ALIMINIUM";
        }
    }

    std::string name = "";

    GLuint      albedo_id        = 0;
    GLuint      metallic_id      = 0;
    GLuint      roughness_id     = 0;
    GLuint      ao_id            = 0;
    GLuint      displacement_id  = 0;
    GLuint      normal_id        = 0;

    float       metallic_strength     = 1.0f;
    float       roughness_strength    = 1.0f;
    float       displacement_strength = 0.1f;
    float       normal_strength       = 1.0f;

    int         reflectance = NONMETAL;
};


// At some point look into storing all model LODs in a single VAO.
// gl::drawArrays(GL_TRIANGLES, lod_start_idx, lod_num_verts);

struct idk::Mesh
{
    int material_id;
    GLuint IBO;
    std::vector<GLuint> vertex_indices;
    Mesh(): material_id(0), IBO(0) {  };
};


struct idk::Model
{
    std::vector<idk::Vertex> vertices;
    std::vector<idk::Mesh> meshes;
    GLuint VAO, VBO;
};

