#pragma once

#include "common.hpp"


namespace idk::ecs
{
    class System
    {
    public:
        virtual void init   ( idk::EngineAPI & ) = 0;
        virtual void update ( idk::EngineAPI & ) = 0;
    };

};
