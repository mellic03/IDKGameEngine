#include "idk_animation_file.hpp"

#include <sstream>
#include <array>



std::vector<std::string> tokenize( std::string str )
{
    std::vector<std::string> tokens;

    std::stringstream ss(str);
    std::string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}


idk::idkvi_header_t
idk::filetools::readheader( const std::string &filepath )
{
    idk::idkvi_header_t header;

    std::ifstream stream(filepath);
    std::string line;

    std::vector<std::string> tokens;

    while (std::getline(stream, line))
    {
        if (line == "animated")
        {
            header.animated = true;
        }

        else if (line != "")
        {
            tokens.push_back(line);
        }
    }

    if (tokens.size() % (TextureIndex::NUM_IDX + 2) != 0)
    {
        std::cout
            << "[idk::filetools::readheader]"
            << "tokens.size() % (TextureIndex::NUM_IDX + 2) != 0\n";

        return header;
    }

    header.num_meshes   = tokens.size() / (TextureIndex::NUM_IDX + 2);

    for (int i=0; i<tokens.size(); i+=TextureIndex::NUM_IDX + 2)
    {
        uint32_t num_indices  = std::stol(tokens[i+0]);
        uint32_t bitmask      = std::stol(tokens[i+1]);

        header.m_index_counts.push_back(num_indices);
        header.m_bitmasks.push_back(bitmask);

        header.m_texture_paths.push_back(std::array<std::string, TextureIndex::NUM_IDX>());
        auto &textures = header.m_texture_paths.back();

        for (int j=0; j<TextureIndex::NUM_IDX; j++)
        {
            if (bitmask & 1<<j)  textures[j] = tokens[i+2+j];
            else                 textures[j] = "";
        }
    }

    return header;
}



void read_animbone( std::ifstream &stream, idk::AnimBone &bone )
{
    uint32_t num_children;
    stream.read(reinterpret_cast<char *>(&num_children), sizeof(uint32_t));

    if (num_children > 0)
    {
        bone.children.resize(num_children);
        stream.read(reinterpret_cast<char *>(&bone.children[0]), num_children*sizeof(int));
    }

    // Read positions
    // -----------------------------------------------------------------------------------------
    uint32_t num_positions;
    stream.read(reinterpret_cast<char *>(&num_positions), sizeof(uint32_t));

    if (num_positions > 0)
    {

        bone.positions.resize(num_positions);
        bone.position_timings.resize(num_positions);

        stream.read(reinterpret_cast<char *>(&bone.positions[0]), num_positions*sizeof(glm::vec3));
        stream.read(reinterpret_cast<char *>(&bone.position_timings[0]), num_positions*sizeof(float));
    }
    // -----------------------------------------------------------------------------------------


    // Read rotations
    // -----------------------------------------------------------------------------------------
    uint32_t num_rotations;
    stream.read(reinterpret_cast<char *>(&num_rotations), sizeof(uint32_t));

    if (num_rotations > 0)
    {
        bone.rotations.resize(num_rotations);
        bone.rotation_timings.resize(num_rotations);

        stream.read(reinterpret_cast<char *>(&bone.rotations[0]), num_rotations*sizeof(glm::quat));
        stream.read(reinterpret_cast<char *>(&bone.rotation_timings[0]), num_rotations*sizeof(float));
    }
    // -----------------------------------------------------------------------------------------


    // Read scales
    // -----------------------------------------------------------------------------------------
    uint32_t num_scales;
    stream.read(reinterpret_cast<char *>(&num_scales), sizeof(uint32_t));

    if (num_scales > 0)
    {
        bone.scales.resize(num_scales);
        bone.scale_timings.resize(num_scales);

        stream.read(reinterpret_cast<char *>(&bone.scales[0]), num_scales*sizeof(glm::vec3));
        stream.read(reinterpret_cast<char *>(&bone.scale_timings[0]), num_scales*sizeof(float));
    }
    // -----------------------------------------------------------------------------------------



    // Read matrices
    // -----------------------------------------------------------------------------------------
    stream.read(reinterpret_cast<char *>(&bone.local_transform), sizeof(glm::mat4));
    stream.read(reinterpret_cast<char *>(&bone.inverse_bind),    sizeof(glm::mat4));
    // -----------------------------------------------------------------------------------------
}


void read_animation( std::ifstream &stream, idk::Animation &animation )
{
    float  duration;
    uint32_t num_bones;

    stream.read(reinterpret_cast<char *>(&duration),  sizeof(float));
    stream.read(reinterpret_cast<char *>(&num_bones), sizeof(uint32_t));

    animation.m_duration = duration;
    animation.m_bones.resize(num_bones);

    // std::cout << "Duration:       " << duration << "\n";
    // std::cout << "No. bones:      " << num_bones << "\n";

    for (idk::AnimBone &bone: animation.m_bones)
    {
        read_animbone(stream, bone);
    }
}


void idk::filetools::readidka( std::ifstream &stream, const idkvi_header_t &header,
                               idk::Animator &controller )
{
    uint32_t num_animations;
    stream.read(reinterpret_cast<char *>(&num_animations), sizeof(uint32_t));
    // std::cout << "No. animations: " << num_animations << "\n";

    controller.m_animations.resize(num_animations);

    for (idk::Animation &animation: controller.m_animations)
    {
        read_animation(stream, animation);
    }
}


void idk::filetools::readidkvi( std::ifstream &stream, const idkvi_header_t &header,
                                idk::iBuffer *vertices, std::vector<uint32_t> &indices )
{
    uint32_t num_vertices, num_indices;
    stream.read(reinterpret_cast<char *>(&num_vertices), sizeof(uint32_t));
    stream.read(reinterpret_cast<char *>(&num_indices),  sizeof(uint32_t));

    vertices->resize(num_vertices);
    indices.resize(num_indices);

    stream.read(reinterpret_cast<char *>(vertices->data()), vertices->nbytes());
    stream.read(reinterpret_cast<char *>(indices.data()),  num_indices * sizeof(uint32_t));
}



