#pragma once

#include "IDKgraphics/IDKgraphics.hpp"
#include "IDKaudio/IDKaudio.hpp"
#include "IDKevents/IDKevents.hpp"

#include <idk_allocator.hpp>
#include "idk_componentsystem.hpp"

#include "idk_threadpool.hpp"


namespace idk { class Engine; };


class idk::Engine
{
private:
    Uint64                                      m_frame_start = 0;
    Uint64                                      m_frame_end   = 0;
    float                                       m_frame_time  = 1.0f;
    bool                                        m_running     = true;

    idk::RenderEngine                           m_render_engine;
    // idk::AudioEngine                            m_audio_engine;
    idk::EventManager                           m_event_manager;

    idk::Allocator<idk::Module *>               m_idk_modules;
    std::unordered_map<std::string, uint>       m_idk_module_ids;

    idk::Allocator<int>                         m_gameobjects;
    std::map<int, std::set<int>>                m_objects_components;   // v[object_id][cs_id]
    std::map<int, std::set<int>>                m_components_objects;   // v[cs_id][object_id]

    idk::Allocator<idk::ComponentSystem *>      m_componentsystems;
    std::unordered_map<size_t, int>             m_componentsystem_ids;

    void                                        _idk_modules_init();
    void                                        _idk_modules_stage_A();
    void                                        _idk_modules_stage_B();
    void                                        _idk_modules_stage_C();

    void                                        idk_CS_checkDependencies( int obj_id, int component_id );
    void                                        idk_CS_onObjectAssignment( int component_id, int obj_id );
    void                                        idk_CS_onObjectCreation( int obj_id );
    void                                        idk_CS_onObjectDeletion( int obj_id );
    void                                        idk_CS_onObjectCopy( int src_obj_id, int dest_obj_id );

public:
                                                // static idk::ThreadPool threadpool;
                                                Engine( std::string name, int w, int h, int res_divisor );

    idk::RenderEngine &                         rengine()   { return m_render_engine; };
    // idk::AudioEngine &                          aengine()   { return m_audio_engine;  };
    idk::EventManager &                         eventManager() { return m_event_manager; };

    bool                                        running()   { return m_running; };
    void                                        beginFrame();
    void                                        endFrame();
    void                                        shutdown();

    float                                       deltaTime() { return m_frame_time;       };
    float                                       frameRate() { return 1.0f / deltaTime(); };

    int                                         createGameObject();
    int                                         createGameObject( int prefab_id );
    void                                        deleteGameObject( int obj_id );
    const Allocator<int> &                      gameObjects() { return m_gameobjects; };
    const std::set<int> &                       gameObjects_byComponent( int component_id );

    void                                        giveComponent( int obj_id, int component_id );
    void                                        giveComponents( int obj_id ) {  };
    template <typename... Args> void            giveComponents( int obj_id,  int, Args... );
    void                                        removeComponent( int obj_id, int component_id );
    bool                                        hasComponent( int obj_id, int component_id );
    template <typename CS> bool                 hasComponent( int obj_id );

    template <typename module_t> int            registerModule( const std::string &name );
    template <typename module_t> module_t &     getModule( int module_id );
    template <typename module_t> module_t &     getModule( const std::string &name );

    template <typename CS> int                  registerCS( const std::string &name );
    template <typename CS> CS &                 getCS( int component_id );
    template <typename CS> CS &                 getCS();

    void                                        initModules() { this->_idk_modules_init(); };

    const Allocator<ComponentSystem *> &        getComponentSystems() { return m_componentsystems; };

};


template <typename CS>
bool 
idk::Engine::hasComponent( int obj_id )
{
    size_t type_idx = typeid(CS).hash_code();
    int    cs_id    = m_componentsystem_ids[type_idx];

    return hasComponent(obj_id, cs_id);
}


template <typename module_t>
int
idk::Engine::registerModule( const std::string &name )
{
    int module_id = m_idk_modules.create(new module_t);

    m_idk_modules.get(module_id)->base_init(module_id, name);
    m_idk_module_ids[name] = module_id;

    return module_id;
}


template <typename module_t>
module_t &
idk::Engine::getModule( int module_id )
{   
    return *dynamic_cast<module_t *>(m_idk_modules.get(module_id));
}


template <typename module_t>
module_t &
idk::Engine::getModule( const std::string &name )
{
    int module_id = m_idk_module_ids[name];
    return *dynamic_cast<module_t *>(m_idk_modules.get(module_id));
}





template <typename... Args>
void
idk::Engine::giveComponents( int obj_id, int first, Args... rest )
{
    giveComponent(obj_id, first);
    giveComponents(obj_id, rest...);
}


template <typename CS>
int
idk::Engine::registerCS( const std::string &name )
{
    int cs_id = m_componentsystems.create(new CS);
    m_componentsystems.get(cs_id)->base_init(cs_id, name);

    size_t type_idx = typeid(CS).hash_code();
    m_componentsystem_ids[type_idx] = cs_id;

    return cs_id;
}


template <typename CS>
CS &
idk::Engine::getCS( int component_id )
{   
    return *dynamic_cast<CS *>(m_componentsystems.get(component_id));
}


template <typename CS>
CS &
idk::Engine::getCS()
{
    size_t type_idx = typeid(CS).hash_code();
    int    cs_id    = m_componentsystem_ids[type_idx];

    return *dynamic_cast<CS *>(m_componentsystems.get(cs_id));
}

