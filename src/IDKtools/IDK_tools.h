#pragma once

#include "../IDKgraphics/IDKgraphics.h"


namespace idktools
{
    void
    vertices_to_bin( std::vector<idk::vertex> &vertices, std::string filepath );

    std::vector<idk::vertex>
    vertices_from_bin( std::string filepath );
};
