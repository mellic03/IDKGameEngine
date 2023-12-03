#pragma once

#include "libidk/libidk.hpp"



/*
    Currently only generates the deferred lighting pass shader.
    Need to make more generic, should also generate the volumetrics shader.
*/

namespace idk::shadergen
{
    struct Config
    {
        std::vector<uint32_t> point_flags;
        std::vector<uint32_t> spot_flags;
        std::vector<uint32_t> dir_flags;
    };

    void genShaderString( const Config &, std::string &vert, std::string &frag );
};

