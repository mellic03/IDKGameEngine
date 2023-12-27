#pragma once

#include <libidk/IDKgl.hpp>
#include <libidk/IDKgl/idk_glDrawCommand.hpp>
#include <idk_buffer.hpp>

#include "idk_vertex.hpp"
#include "idk_OBB.hpp"

#include <vector>


namespace idk
{
    struct Material;
    struct Mesh;
    struct Model;

    void loadChunked( idk::Model &model, const std::vector<glm::vec4> &positions,
                      const std::vector<idk::OBB> &OBBs,
                      const std::vector<std::vector<glm::mat4>> &transforms );

    void genDrawCommands( idk::Model &model, const glm::mat4 &transform,
                          float cam_near, float cam_far,
                          const glm::mat4 &cam_proj, const glm::mat4 &cam_view );


    enum ModelRenderFlag: uint32_t
    {
        NONE      = 0 << 0,
        ANIMATED  = 1 << 0,
        INSTANCED = 1 << 1,
        CHUNKED   = 1 << 2
    };
};




struct idk::Material
{
    std::string name = "";

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


struct idk::Mesh
{
    int material_id;
    GLuint num_indices;
};


struct idk::Model
{
    uint32_t render_flags = ModelRenderFlag::NONE;

    std::vector<idk::Mesh>    meshes;
    idk::iBuffer             *m_vertices = nullptr;
    std::vector<uint32_t>     m_indices;

    idk::OBB m_OBB;

    // Animation
    // ---------------------------------------------------
    int  animator_id = -1;
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

