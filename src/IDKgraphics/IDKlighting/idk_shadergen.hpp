#pragma once

#include "IDKcommon/IDKcommon.h"



/*
    Currently only generates the deferred lighting pass shader.
    Need to make more generic, should also generate the volumetrics shader.
*/

namespace idk::shadergen
{
    struct Config
    {
        idk::vector<uint32_t> point_flags;
        idk::vector<uint32_t> spot_flags;
        idk::vector<uint32_t> dir_flags;
    };

    void genShaderString( const Config &, std::string &vert, std::string &frag );
};

