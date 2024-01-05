#pragma once

#include <map>
#include <set>

// #include "IDKgraphics/IDKgraphics.hpp"
#include "IDKaudio/IDKaudio.hpp"
#include "IDKevents/IDKevents.hpp"

#include <libidk/idk_module.hpp>
#include <libidk/idk_componentsystem.hpp>
#include <libidk/idk_allocator.hpp>
#include <libidk/idk_singleton.hpp>


namespace idk { class Engine;       };
namespace idk { class RenderEngine; };
namespace idk { class EngineAPI;    };

namespace idk::internal { class EngineAPI; };



class IDK_VISIBLE idk::Engine
{
private:
    friend class idk::EngineAPI;
    friend class idk::internal::EngineAPI;

    idk::EngineAPI *APIptr = nullptr;

    Uint64                                      m_frame_start = 0;
    Uint64                                      m_frame_end   = 0;
    float                                       m_frame_time  = 1.0f;
    bool                                        m_running     = true;

    idk::AudioEngine *                          m_audio_engine;
    idk::EventManager                           m_event_manager;


    idk::Allocator<idk::Module *>               m_idk_modules;
    std::unordered_map<std::string, uint>       m_idk_module_ids;

    idk::Allocator<int>                         m_gameobjects;
    std::map<int, std::set<int>>                m_objects_components;   // v[object_id][cs_id]
    std::map<int, std::set<int>>                m_components_objects;   // v[cs_id][object_id]

    idk::Allocator<idk::ComponentSystem *>      m_componentsystems;
    std::unordered_map<size_t, int>             m_componentsystem_ids;
    std::unordered_map<int,    std::string>     m_componentsystem_names;


    void                                        _idk_modules_init();
    void                                        _idk_modules_stage_A();
    void                                        _idk_modules_stage_B();
    void                                        _idk_modules_stage_C();

    void                                        idk_CS_checkDependencies( int obj_id, int component_id );
    void                                        idk_CS_onObjectAssignment( int component_id, int obj_id );
    void                                        idk_CS_onObjectCreation( int obj_id );
    void                                        idk_CS_onObjectDeletion( int obj_id );
    void                                        idk_CS_onObjectCopy( int src_obj_id, int dest_obj_id );

    idk::AudioEngine &                          _audio_engine()  { return *m_audio_engine;  };

    bool    _running() { return m_running; };
    void    _begin_frame( idk::RenderEngine & );
    void    _end_frame( idk::RenderEngine & );

public:

    struct Config
    {
        const char *name;

        size_t      width;
        size_t      height;

        uint32_t    gl_major;
        uint32_t    gl_minor;
    };

    Engine( idk::RenderEngine & );


    idk::EventManager &                         eventManager() { return m_event_manager; };

    bool                                        running()   { return m_running; };
    void                                        shutdown();

    float                                       deltaTime() { return m_frame_time;       };
    float                                       frameRate() { return 1.0f / deltaTime(); };

    int                                         createGameObject( const std::string &name = "Object" );
    int                                         copyGameObject( int obj_id, const std::string &name = "Object" );
    void                                        deleteGameObject( int obj_id );
    const Allocator<int> &                      gameObjects() { return m_gameobjects; };
    const std::set<int> &                       gameObjects_byComponent( int component_id );



    void                                        giveComponent( int obj_id, int component_id );

    template <typename CS_type>
    void                                        giveComponent( int obj_id );

    void                                        giveComponents( int obj_id ) {  };
    template <typename... Args> void            giveComponents( int obj_id,  int, Args... );

    void                                        removeComponent( int obj_id, int component_id );

    bool                                        hasComponent( int obj_id, int component_id );
    template <typename CS_type> bool            hasComponent( int obj_id );



    template <typename module_type> int         registerModule( const std::string &name );
    int                                         registerModule( const std::string &name, const std::string &filepath );

    template <typename module_type> module_type &  getModule( int module_id );
    template <typename module_type> module_type &  getModule( const std::string &name );

    template <typename CS_type> int             registerCS( const std::string &name );
    template <typename CS_type> CS_type &       getCS( int component_id );
    template <typename CS_type> CS_type &       getCS();
    idk::ComponentSystem *                      getCS( int component_id )
    {
        return m_componentsystems.get(component_id);
    };

    const Allocator<ComponentSystem *> &        getComponentSystems() { return m_componentsystems; };

};


template <typename CS_type>
bool 
idk::Engine::hasComponent( int obj_id )
{
    size_t type_idx = typeid(CS_type).hash_code();
    int    cs_id    = m_componentsystem_ids[type_idx];

    return hasComponent(obj_id, cs_id);
}


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
void
idk::Engine::giveComponent( int obj_id )
{
    int cs_id = getCS<CS_type>().ID();

    if (hasComponent(obj_id, cs_id))
    {
        std::cout
            << "Warning, object " << obj_id
            << " already has component with ID " << cs_id
            << "\n";
        return;
    }

    giveComponent(obj_id, cs_id);
}



template <typename... Args>
void
idk::Engine::giveComponents( int obj_id, int first, Args... rest )
{
    giveComponent(obj_id, first);
    giveComponents(obj_id, rest...);
}


template <typename CS_type>
int
idk::Engine::registerCS( const std::string &name )
{
    int cs_id = m_componentsystems.create(new CS_type);
    m_componentsystems.get(cs_id)->base_init(cs_id, name);

    size_t type_idx = typeid(CS_type).hash_code();
    m_componentsystem_ids[type_idx] = cs_id;
    m_componentsystem_names[cs_id] = name;

    return cs_id;
}


template <typename CS_type>
CS_type &
idk::Engine::getCS( int component_id )
{   
    return *dynamic_cast<CS_type *>(m_componentsystems.get(component_id));
}


template <typename CS_type>
CS_type &
idk::Engine::getCS()
{
    size_t type_idx = typeid(CS_type).hash_code();
    int    cs_id    = m_componentsystem_ids[type_idx];

    return *dynamic_cast<CS_type *>(m_componentsystems.get(cs_id));
}

