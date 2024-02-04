#pragma once

#include "common.hpp"


namespace idecs
{
    static constexpr size_t MAX_COMPONENTS = 64;

    struct Entity
    {
        char    name[64];
        int     components[idecs::MAX_COMPONENTS];
    };


    inline Entity
    createEntity( const std::string &name = "Empty" )
    {
        Entity E;

        std::strcpy(E.name, name.c_str());
        std::fill_n(E.components, idecs::MAX_COMPONENTS, -1);

        return E;
    }

};



