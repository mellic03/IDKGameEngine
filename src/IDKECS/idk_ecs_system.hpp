#pragma once

#include "common.hpp"


namespace idecs
{
    class System
    {
    public:
        virtual void init   ( idk::EngineAPI & ) = 0;
        virtual void update ( idk::EngineAPI & ) = 0;
    };

};
