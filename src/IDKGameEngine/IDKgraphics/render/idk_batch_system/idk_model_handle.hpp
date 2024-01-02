#pragma once

#include <libidk/GL/common.hpp>


namespace idk
{
    struct ModelHandle
    {
        uint32_t vertexformat  = 0;

        int      batch_id      = -1;
        int      base_vertex   = -1;
        int      base_instance = -1;
    };
};
