#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>



namespace idk
{
    class TransformSys;
    // class PhysicsSys;
    class ModelSys;
    class ScriptSys;
    class CameraSys;

};



class idk::TransformSys: public idecs::System, public idk::LuaAPI
{
public:
    virtual void        init   ( idk::EngineAPI & ) final;
    virtual void        update ( idk::EngineAPI & ) final {  };

    virtual void        exposeToLua( lua_State *LS ) final;

    
    static idk::TransformCmp &getData( int obj_id );


    static void         pitch ( int obj_id, float f );
    static void         yaw   ( int obj_id, float f );
    static void         yaw_global( int obj_id, float f );
    static void         lookTowards( int subject, int target, float alpha );

    // static glm::vec3    front ( idecs::ECS &ecs, int obj_id );
    // static glm::vec3    right ( idecs::ECS &ecs, int obj_id );

    static void         translate( int obj_id, const glm::vec3 & );
    static void         translate_local( int obj_id, const glm::vec3 & );


    static glm::vec3   &getPosition      ( int obj_id );
    static glm::vec3    getWorldPosition ( int obj_id );
    static glm::vec3    getFront         ( int obj_id );
    static glm::mat4    getModelMatrix   ( int obj_id, bool scale = true );
    static glm::mat4    getWorldMatrix   ( int obj_id );

};



// class idk::PhysicsSys: public idecs::System
// {
// public:
//     virtual void    init   ( idk::EngineAPI & ) final {  };
//     virtual void    update ( idk::EngineAPI & ) final;

// };



class idk::ModelSys: public idecs::System
{
public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    static void     assignModel( int obj_id, int model );

};


class idk::ScriptSys: public idecs::System
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
    int     assignScript( int obj_id, const std::string &filepath );

};



class idk::CameraSys: public idecs::System
{
public:
    virtual void    init   ( idk::EngineAPI & ) final {  };
    virtual void    update ( idk::EngineAPI & ) final;

    static bool     in_frustum ( int subject, int target );

};


