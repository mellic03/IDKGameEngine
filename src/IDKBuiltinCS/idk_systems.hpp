#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>



namespace idk
{
    class ScriptSys;
    class CameraSys;
    class LightSys;
};



// class idk::PhysicsSys: public idk::ECS2::System
// {
// public:
//     virtual void    init   ( idk::EngineAPI & ) final {  };
//     virtual void    update ( idk::EngineAPI & ) final;

// };



class idk::CameraSys: public idk::ECS2::System
{
public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    static bool     in_frustum ( int subject, int target );
};


class idk::LightSys: public idk::ECS2::System
{
public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

};


