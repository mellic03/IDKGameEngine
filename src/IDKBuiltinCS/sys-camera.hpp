#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>



namespace idk
{
    class CameraSys;
};


class idk::CameraSys: public idk::ECS2::System
{
public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    static bool     in_frustum ( int subject, int target );
};


