#pragma once

#include <map>
#include <set>
#include <deque>
#include <memory>

#include <libidk/idk_module.hpp>
#include <libidk/idk_componentsystem.hpp>
#include <libidk/idk_allocator.hpp>
#include <libidk/idk_dynamiclib.hpp>
#include <libidk/idk_scripting.hpp>

#include <cstdint>
#include <cstddef>
#include <unordered_map>


namespace idk { class Engine;       };
namespace idk { class RenderEngine; };
namespace idk { class EventSystem;  };


class IDK_VISIBLE idk::Engine: public idk::LuaAPI
{
private:
    using ModuleLoader = idk::GenericLoader<idk::Module>;

    uint64_t                                    m_frame_start = 0;
    uint64_t                                    m_frame_end   = 0;
    float                                       m_frame_time  = 0.0001f;
    bool                                        m_running     = true;
    bool                                        m_reload      = false;

    idk::Allocator<idk::Module *>               m_idk_modules;
    std::unordered_map<std::string, uint>       m_idk_module_ids;
    idk::Allocator<ModuleLoader>                m_module_loaders;

    std::deque<int>                             m_selection;
    idk::Allocator<int>                         m_gameobjects;
    idk::Allocator<std::string>                 m_gameobject_names;

    idk::Allocator<idk::ComponentSystem *>      m_componentsystems;
    std::map<int, std::map<int, bool>>          m_component_matrix;
    std::unordered_map<size_t, int>             m_componentsystem_ids;
    std::unordered_map<int, std::string>        m_componentsystem_names;


    void                                        _idk_modules_stage_A( idk::EngineAPI & );
    void                                        _idk_modules_stage_B( idk::EngineAPI & );
    void                                        _idk_modules_stage_C( idk::EngineAPI & );
    void                                        _reloadModules();

    // void                                        idk_CS_onObjectAssignment   ( int component_id, int obj_id );
    // void                                        idk_CS_onObjectDeassignment ( int component_id, int obj_id );
    // void                                        idk_CS_onObjectCreation     ( int obj_id );
    // void                                        idk_CS_onObjectDeletion     ( int obj_id );
    // void                                        idk_CS_onObjectCopy         ( int src_id, int dst_id );


public:
    // static constexpr float TARGET_FPS = 165.0;
    // static constexpr float TARGET_DT  = 1.0 / TARGET_FPS;


    void                                        initModules( idk::EngineAPI & );
    void                                        reloadModules() { m_reload = true; };

    void                                        beginFrame  ( idk::EngineAPI&, float );
    void                                        endFrame    ( idk::EngineAPI& );

    bool                                        running() { return m_running; };
    void                                        shutdown();

    float                                       deltaTime() { return m_frame_time;       };
    float                                       frameRate() { return 1.0f / deltaTime(); };



    template <typename module_type> int         registerModule( const std::string &name );
    int                                         registerModule( const std::string &name, const std::string &filepath );

    template <typename module_type> module_type &  getModule( int module_id );
    template <typename module_type> module_type &  getModule( const std::string &name );

    template <typename CS_type> int             registerCS( const std::string &name );
    template <typename CS_type> CS_type &       getCS( int component_id );
    template <typename CS_type> CS_type &       getCS();
    idk::ComponentSystem *                      getCS( int component_id );

    const Allocator<ComponentSystem *> &        getComponentSystems() { return m_componentsystems; };



    void exposeToLua( lua_State *L )
    {
        luaaa::LuaModule mod1(L, "Engine");
        mod1.fun("deltaTime", [this]() { return deltaTime(); });
    };


};



template <typename module_type>
int
idk::Engine::registerModule( const std::string &name )
{
    int module_id = m_idk_modules.create(new module_type);

    m_idk_modules.get(module_id)->base_init(module_id, name);
    m_idk_module_ids[name] = module_id;

    return module_id;
}


template <typename module_type>
module_type &
idk::Engine::getModule( int module_id )
{   
    return *dynamic_cast<module_type *>(m_idk_modules.get(module_id));
}


template <typename module_type>
module_type &
idk::Engine::getModule( const std::string &name )
{
    int module_id = m_idk_module_ids[name];
    return *dynamic_cast<module_type *>(m_idk_modules.get(module_id));
}


template <typename CS_type>
int
idk::Engine::registerCS( const std::string &name )
{
    int cs_id = m_componentsystems.create(new CS_type);
    m_componentsystems.get(cs_id)->base_init(cs_id, name);

    auto type_idx = std::type_index(typeid(CS_type)).hash_code();
    m_componentsystem_ids[type_idx] = cs_id;
    m_componentsystem_names[cs_id] = name;

    return cs_id;
}


template <typename CS_type>
CS_type &
idk::Engine::getCS( int component_id )
{   
    return *dynamic_cast<CS_type *>(getCS(component_id));
}


template <typename CS_type>
CS_type &
idk::Engine::getCS()
{
    auto type_idx = std::type_index(typeid(CS_type)).hash_code();
    int  cs_id    = m_componentsystem_ids[type_idx];
    return *dynamic_cast<CS_type *>(getCS(cs_id));
}

