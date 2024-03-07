#pragma once

#include "common.hpp"


namespace idk::ecs
{
    static constexpr size_t MAX_COMPONENTS = 64;

    struct Entity
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( std::string,       name,       ""                 ) \
            X( std::vector<int>,  components, std::vector<int>(MAX_COMPONENTS, -1) )

        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS
    };


    inline Entity
    createEntity( const std::string &name = "Empty" )
    {
        Entity E;

        E.name = name;
        E.components.resize(idk::ecs::MAX_COMPONENTS, -1);

        return E;
    }

};



