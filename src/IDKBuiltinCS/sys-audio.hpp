#pragma once

#include "idk_components.hpp"


namespace idk
{
    class AudioSys;
};


class idk::AudioSys: public idecs::System, public idk::LuaAPI
{
public:
    virtual void        init   ( idk::EngineAPI & ) final;
    virtual void        update ( idk::EngineAPI & ) final;

    virtual void        exposeToLua( lua_State *LS ) final;

    static int          loadSound   ( const std::string &filepath );
    static void         assignSound ( int obj_id, int sound );
    static void         assignSound ( int obj_id, const std::string &filepath );

    static void         playSound ( int obj_id );
    static void         pauseSound( int obj_id );
    static void         stopSound ( int obj_id );

};

