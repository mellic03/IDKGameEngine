#include "idk_filetools.h"

#include <sstream>
#include <libidk/libidk.hpp>


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



void read_animbone( std::ifstream &stream, idk::AnimBone &animbone )
{
    stream.read(reinterpret_cast<char *>(&animbone.parent_id), sizeof(int));
    stream.read(reinterpret_cast<char *>(&animbone.world_transform), sizeof(glm::mat4));
    stream.read(reinterpret_cast<char *>(&animbone.local_transform), sizeof(glm::mat4));
    stream.read(reinterpret_cast<char *>(&animbone.inverse_bind),    sizeof(glm::mat4));


    size_t num_pos=0, num_rot=0, num_scale=0;
    stream.read(reinterpret_cast<char *>(&num_pos),   sizeof(size_t));
    stream.read(reinterpret_cast<char *>(&num_rot),   sizeof(size_t));
    stream.read(reinterpret_cast<char *>(&num_scale), sizeof(size_t));

    // std::cout << "p/r/s: " << num_pos << ", " << num_rot << ", " << num_scale << "\n";


    animbone.position.resize(num_pos);
    animbone.rotation.resize(num_rot);
    animbone.scale.resize(num_scale);

    animbone.position_time.resize(num_pos);
    animbone.rotation_time.resize(num_rot);
    animbone.scale_time.resize(num_scale);

    if (num_pos > 0)
    {
        stream.read(reinterpret_cast<char *>(&animbone.position[0]),      num_pos*sizeof(glm::vec3));
        stream.read(reinterpret_cast<char *>(&animbone.position_time[0]), num_pos*sizeof(float));
    }

    if (num_rot > 0)
    {
        stream.read(reinterpret_cast<char *>(&animbone.rotation[0]),      num_rot*sizeof(glm::quat));
        stream.read(reinterpret_cast<char *>(&animbone.rotation_time[0]), num_rot*sizeof(float));
    }

    if (num_scale > 0)
    {
        stream.read(reinterpret_cast<char *>(&animbone.scale[0]),         num_scale*sizeof(glm::vec3));
        stream.read(reinterpret_cast<char *>(&animbone.scale_time[0]),    num_scale*sizeof(float));
    }

}


void read_animation( std::ifstream &stream, idk::Animation &animation )
{
    size_t num_animbones;
    float  duration;

    stream.read(reinterpret_cast<char *>(&num_animbones), sizeof(size_t));
    stream.read(reinterpret_cast<char *>(&duration),      sizeof(float));

    animation.m_bones.resize(num_animbones);
    animation.m_duration = duration;

    std::cout << "bones: " << num_animbones << ", duration: " << duration << "\n";

    for (size_t i=0; i<num_animbones; i++)
    {
        read_animbone(stream, animation.m_bones[i]);
    }
}



void idk::filetools::readidka( const idkvi_header_t &header, const std::string &filepath,
                               std::vector<idk::Animation> &animations )
{
    std::ifstream stream(filepath, std::ios::binary);

    size_t num_animations;
    stream.read(reinterpret_cast<char *>(&num_animations), sizeof(size_t));
    std::cout << "Num animations: " << num_animations << "\n";


    animations.resize(num_animations);

    for (int i=0; i<num_animations; i++)
    {
        read_animation(stream, animations[i]);
        animations[i].init();
    }


    stream.close();

}

