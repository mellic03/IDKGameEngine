#pragma once

#include <libidk/idk_gl.hpp>

#include <libidk/containers/idk_buffer.hpp>
#include <libidk/idk_export.hpp>

#include "idk_vertex.hpp"
#include "idk_OBB.hpp"

#include <vector>
#include <memory>




namespace idk
{
    struct Material;
    struct Mesh;
    struct Model;

    // Model types
    // -----------------------------------------------------------------------------------------
    struct Model_Regular;
    struct Model_Animated;
    struct Model_Terrain;
    struct Model_Chunked;
    // -----------------------------------------------------------------------------------------


    void loadChunked( idk::Model &model, const std::vector<glm::vec4> &positions,
                      const std::vector<std::vector<glm::mat4>> &transforms );

    void genDrawCommands( idk::Model &model, const glm::mat4 &transform,
                          float cam_near, float cam_far,
                          const glm::mat4 &cam_proj, const glm::mat4 &cam_view );


    enum ModelRenderFlag: uint32_t
    {
        NONE         = 0 << 0,
        ANIMATED     = 1 << 0,
        INSTANCED    = 1 << 1,
        CHUNKED      = 1 << 2,
        HEIGHTMAPPED = 1 << 3
    };
};



struct IDK_VISIBLE idk::Material
{
    std::string name = "";

    GLuint      bindless_idx     = 0;

    GLuint      albedo_id        = 0;
    GLuint      arm_id           = 0;
    GLuint      ao_id            = 0;
    GLuint      displacement_id  = 0;
    GLuint      normal_id        = 0;

    float       metallic_strength     = 1.0f;
    float       roughness_strength    = 1.0f;
    float       displacement_strength = 0.1f;
    float       normal_strength       = 1.0f;
};



struct IDK_VISIBLE idk::Mesh
{
    int material_id;
    GLuint num_indices;
};


struct IDK_VISIBLE idk::Model_Regular
{
    std::vector<idk::Mesh> meshes;

};


struct IDK_VISIBLE idk::Model_Animated
{
    std::vector<idk::AnimatedVertex> vertices;

};

/** Terrain model types have only one mesh -the subdivided plane.
 *  Only one heightmap texture is used, referenced by heightmap_id.
 *  There can be up to four materials for texture splatting/blending 
*/
struct IDK_VISIBLE idk::Model_Terrain
{
    std::unique_ptr<uint8_t[]> heightmap_data;

    GLuint      heightmap_id     = 0;
    GLuint64    heightmap_handle = 0;
    float       height_scale     = 25.0f;
    float       world_scale      = 50.0f;

    size_t      num_materials = 0;
    int         material_ids[4];
};


struct IDK_VISIBLE idk::Model
{
    uint32_t render_flags = ModelRenderFlag::NONE;

    std::vector<idk::Mesh>    meshes;
    idk::iBuffer             *m_vertices = nullptr;
    std::vector<uint32_t>     m_indices;

    idk::OBB m_OBB;

    // Model_Regular
    // ---------------------------------------------------
    int regular_id = -1;
    // ---------------------------------------------------

    // Model_Animated
    // ---------------------------------------------------
    int animated_id = -1;
    // ---------------------------------------------------

    // Model_Terrain
    // ---------------------------------------------------
    int terrain_id = -1;
    // ---------------------------------------------------

    // Animation
    // ---------------------------------------------------
    int animator_id = -1;
    // ---------------------------------------------------

    // Instancing
    // ---------------------------------------------------
    glInstancedTransforms     m_instancedata;
    // ---------------------------------------------------

    // Chunking
    // ---------------------------------------------------
    GLuint m_IDB; // Indirect Draw Buffer

    std::vector<glm::vec4>   m_chunk_positions;
    std::vector<glm::mat4>   m_chunk_transforms;
    std::vector<idk::OBB>    m_chunk_OBBs;

    std::vector<glDrawElementsIndirectCommand> m_default_draw_params;
    std::vector<glDrawElementsIndirectCommand> m_draw_params;
    // ---------------------------------------------------


    GLuint VAO, VBO, IBO;
};

