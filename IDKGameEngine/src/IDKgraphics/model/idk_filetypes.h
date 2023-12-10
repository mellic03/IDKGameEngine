#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <fstream>

#include "idk_vertex.hpp"
#include "libidk/libidk.hpp"


namespace idk
{
    struct idkvi_header_t;
    struct idkvi_material_t;

    enum TextureIndex { ALBEDO_IDX, NORMAL_IDX, RM_IDX, AO_IDX, NUM_IDX };
    enum TextureBit   { ALBEDO_BIT=1<<0, NORMAL_BIT=1<<1, RM_BIT=1<<2, AO_BIT=1<<3 };
};


struct idk::idkvi_header_t
{
    bool   animated = false;
    size_t num_meshes;

    std::vector<uint32_t> m_index_counts;
    std::vector<uint32_t> m_bitmasks;

    std::vector<std::array<std::string, TextureIndex::NUM_IDX>> m_texture_paths;
};


struct idk::idkvi_material_t
{
    glm::vec3 reflectance;
};
