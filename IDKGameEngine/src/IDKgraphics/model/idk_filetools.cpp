#include "idk_filetools.h"

#include <sstream>


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
        if (line != "")
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


void
idk::filetools::readidkvi( const idk::idkvi_header_t &header, const std::string &filepath,
                           std::vector<idkvi_material_t> &materials,
                           idk::iBuffer *vertices, idk::iBuffer *indices )
{
    std::ifstream stream(filepath, std::ios::binary);

    size_t num_materials, num_vertices, num_indices;
    stream.read(reinterpret_cast<char *>(&num_materials), sizeof(size_t));
    stream.read(reinterpret_cast<char *>(&num_vertices), sizeof(size_t));
    stream.read(reinterpret_cast<char *>(&num_indices),  sizeof(size_t));

    materials.resize(num_materials);
    vertices->resize(num_vertices);
    indices->resize(num_indices);

    stream.read(reinterpret_cast<char *>(materials.data()), materials.size() * sizeof(idkvi_material_t));
    stream.read(reinterpret_cast<char *>(vertices->data()), vertices->nbytes());
    stream.read(reinterpret_cast<char *>(indices->data()),  indices->nbytes());

    stream.close();
}

