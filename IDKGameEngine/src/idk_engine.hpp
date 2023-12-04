#pragma once

#include "IDKgraphics/IDKgraphics.hpp"
#include "IDKaudio/IDKaudio.hpp"
#include "IDKevents/IDKevents.hpp"
#include "idk_componentsystem.hpp"

// #include "idk_threadpool.hpp"

namespace idk { class Engine; };


class idk::Engine
{
private:
    Uint64                                      m_frame_start = 0;
    Uint64                                      m_frame_end   = 0;
    float                                       m_frame_time  = 1.0f;
    bool                                        m_running     = true;

    idk::RenderEngine                           m_render_engine;
    idk::AudioEngine                            m_audio_engine;
    idk::EventManager                           m_event_manager;

    std::vector<idk::Module *>                  m_idk_modules;
    std::unordered_map<std::string, uint>       m_idk_module_ids;

    idk::Allocator<int>                         m_gameobjects;
    std::set<int>                               m_object_ids;
    std::map<int, std::set<int>>                m_objects_components;   // v[object_id][cs_id]
    std::map<int, std::set<int>>                m_components_objects;   // v[cs_id][object_id]

    std::vector<idk::ComponentSystem *>         m_componentsystems;
    std::unordered_map<std::string, uint>       m_componentsystem_ids;  // map[cs_name] = cs_id;

    void                                        idk_CS_stage_A();
    void                                        idk_CS_stage_B();
    void                                        idk_CS_stage_C();
    void                                        idk_CS_checkDependencies( int obj_id, int component_id );
    void                                        idk_CS_onObjectAssignment( int component_id, int obj_id );
    void                                        idk_CS_onObjectCreation( int obj_id );
    void                                        idk_CS_onObjectDeletion( int obj_id );
    void                                        idk_CS_onObjectCopy( int src_obj_id, int dest_obj_id );

public:
                                                // static idk::ThreadPool threadpool;
                                                Engine( std::string name, int w, int h, int res_divisor );

    idk::RenderEngine &                         rengine()   { return m_render_engine; };
    idk::AudioEngine &                          aengine()   { return m_audio_engine;  };
    idk::EventManager &                         eventManager() { return m_event_manager; };

    bool                                        running()   { return m_running; };
    void                                        beginFrame();
    void                                        endFrame();
    void                                        shutdown();

    float                                       deltaTime() { return m_frame_time;        };
    float                                       frameRate() { return 1.0f / deltaTime(); };

    int                                         createGameObject();
    int                                         createGameObject( int prefab_id );
    void                                        deleteGameObject( int obj_id );
    const std::set<int> &                       gameObjects();
    const std::set<int> &                       gameObjects_byComponent( int component_id );

    void                                        giveComponent( int obj_id, int component_id );
    void                                        giveComponents( int obj_id ) {  };
    template <typename... Args> void            giveComponents( int obj_id,  int, Args... );
    void                                        removeComponent( int obj_id, int component_id );
    bool                                        hasComponent( int obj_id, int component_id );
    bool                                        hasComponent( int obj_id, std::string component_name );

    template <typename module_t> int            registerModule( std::string name );
    template <typename module_t> module_t &     getModule( int module_id );
    template <typename module_t> module_t &     getModule( std::string name );

    template <typename comp_t> int              registerCS( std::string name );
    template <typename comp_t> comp_t &         getCS( int component_id );
    template <typename comp_t> comp_t &         getCS( std::string name );
};




template <typename module_t>
int
idk::Engine::registerModule( std::string name )
{   
    int module_id = m_idk_modules.size();
    m_idk_modules.push_back(new module_t());
    m_idk_modules.back()->base_init(m_idk_modules.size()-1, name);
    m_idk_modules.back()->init(*this);
    m_idk_module_ids[name] = module_id;
    return module_id;
}


template <typename module_t>
module_t &
idk::Engine::getModule( int component_id )
{   
    return *dynamic_cast<module_t *>(m_idk_modules[component_id]);
}


template <typename module_t>
module_t &
idk::Engine::getModule( std::string name )
{
    int module_id = m_idk_module_ids[name];
    return *dynamic_cast<module_t *>(m_idk_modules[module_id]);
}






template <typename... Args>
void
idk::Engine::giveComponents( int obj_id, int first, Args... rest )
{
    giveComponent(obj_id, first);
    giveComponents(obj_id, rest...);
}


template <typename comp_t>
int
idk::Engine::registerCS( std::string name )
{   
    int cs_id = m_componentsystems.size();
    m_componentsystems.push_back(new comp_t());
    m_componentsystems.back()->base_init(m_componentsystems.size()-1, name);
    m_componentsystems.back()->init(*this);
    m_componentsystem_ids[name] = cs_id;
    return cs_id;
}


template <typename comp_t>
comp_t &
idk::Engine::getCS( int component_id )
{   
    return *dynamic_cast<comp_t *>(m_componentsystems[component_id]);
}


template <typename comp_t>
comp_t &
idk::Engine::getCS( std::string name )
{
    int cs_id = m_componentsystem_ids[name];
    return *dynamic_cast<comp_t *>(m_componentsystems[cs_id]);
}
