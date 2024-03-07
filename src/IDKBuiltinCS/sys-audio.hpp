#pragma once

#include "idk_components.hpp"
#include "sys-transform.hpp"


namespace idk
{
    class AudioSys;
};


class idk::AudioSys: public idk::ecs::System, public idk::LuaAPI
{
    inline static std::unordered_map<int, std::function<void()>> m_callbacks;

public:
    virtual void        init   ( idk::EngineAPI & ) final;
    virtual void        update ( idk::EngineAPI & ) final;

    virtual void        exposeToLua( lua_State *LS ) final;

    static int          loadSound   ( const std::string &filepath );
    static void         assignSound ( int obj_id, int sound );
    static void         assignSound ( int obj_id, const std::string &filepath );

    static void         playSoundCallback ( int obj_id, std::function<void()>);
    static void         playSound         ( int obj_id, bool loop );
    static void         stopSound         ( int obj_id );
    static void         resumeSound       ( int obj_id );
    static void         pauseSound        ( int obj_id );

    static bool         isFinished( int obj_id );
    static bool         isPlaying( int obj_id );




};

