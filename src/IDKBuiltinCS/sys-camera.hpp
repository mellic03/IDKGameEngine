#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>


namespace idk
{
    class CameraSys;
};


class idk::CameraSys: public idk::ECS::System
{
public:
    virtual void init   ( idk::EngineAPI & ) final;
    virtual void update ( idk::EngineAPI & ) final;

    float &getFov       ( int obj_id );
    float &getFovOffset ( int obj_id );
    bool  in_frustum    ( int subject, int target );
};


