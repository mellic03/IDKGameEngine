#pragma once

#include "../IDKengine/IDKgraphics.h"


namespace idktools
{
    void
    vertices_to_bin( std::vector<idk::Vertex> &vertices, std::string filepath );

    std::vector<idk::Vertex>
    vertices_from_bin( std::string filepath );
};
