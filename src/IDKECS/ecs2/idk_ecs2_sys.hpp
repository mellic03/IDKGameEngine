#pragma once

#include "idk_ecs2.hpp"
#include <libidk/idk_serialize.hpp>


class idk::ECS2::iSystem
{
protected:
    std::string m_name;

public:
    iSystem( const std::string &name )
    :   m_name(name)
    {

    }

    virtual size_t serialize   ( std::ofstream& ) = 0;
    virtual size_t deserialize ( std::ifstream& ) = 0;


};




