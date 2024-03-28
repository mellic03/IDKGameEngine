#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>



namespace idk
{
    class ModelSys;
    class ScriptSys;
    class CameraSys;
    class LightSys;
};



// class idk::PhysicsSys: public idk::ecs::System
// {
// public:
//     virtual void    init   ( idk::EngineAPI & ) final {  };
//     virtual void    update ( idk::EngineAPI & ) final;

// };



class idk::ModelSys: public idk::ecs::System
{
public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    static void     assignModel( int obj_id, const std::string &filepath );
    static void     assignShader_gpass( int obj_id, const std::string &shader_name );

};


class idk::ScriptSys: public idk::ecs::System
{
private:
    lua_State *LS;
    idk::vector<std::string> m_scripts;


public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;


    void    loadScript  ( const std::string &filepath  );
    void    loadScripts ( const std::string &directory );
    auto   &getScripts() { return m_scripts; };
    static int assignScript( int obj_id, const std::string &filepath );

};



class idk::CameraSys: public idk::ecs::System, public idk::LuaAPI
{
public:
    virtual void    init   ( idk::EngineAPI & ) final {  };
    virtual void    update ( idk::EngineAPI & ) final;
    virtual void    exposeToLua( lua_State *LS ) final;

    static bool     in_frustum ( int subject, int target );
};


class idk::LightSys: public idk::ecs::System
{
public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

};


