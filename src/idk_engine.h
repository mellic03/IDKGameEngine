#pragma once

#include "IDKgraphics/IDKgraphics.h"
#include "idk_componentsystem.h"

namespace idk { class Engine; };


class idk::Engine
{
private:
    Uint64                                      _frame_start;
    Uint64                                      _frame_end;
    float                                       _frame_time;
    bool                                        _running = true;

    idk::RenderEngine                           _render_engine;
    SDL_Event                                   _SDL_event;
    idk::Keylog                                 _keylog;
    idk::vector<bool>                           _mouse_down;

    glm::vec2                                   _delta_mouse_position;
    glm::vec2                                   _mouse_position;
    bool                                        _mouse_captured;

    idk::Allocator<int>                         _gameobjects;
    std::vector<std::vector<int>>               _component_matrix; // v[object_id][cs_id]
    std::vector<idk::ComponentSystem *>         _idk_componentsystems;
    std::unordered_map<std::string, uint>       _idk_componentsystem_ids; // map[cs_name] = cs_id;

    void                                        _process_key_input();
    void                                        _process_mouse_input();

    void                                        _idk_CS_stage_A();
    void                                        _idk_CS_stage_B();
    void                                        _idk_CS_onCreation( int obj_id );
    void                                        _idk_CS_onDeletion( int obj_id );
    void                                        _idk_CS_onCopy( int src_obj_id, int dest_obj_id );
    void                                        _idk_CS_onAssignment( int component_id, int obj_id );

public:
                                                Engine( size_t w = 1000, size_t h = 1000 );

    idk::RenderEngine &                         rengine()   { return _render_engine; };
    Keylog &                                    keylog()    { return _keylog; };

    bool                                        running()   { return _running; };
    void                                        beginFrame();
    void                                        endFrame();
    void                                        shutdown();

    float                                       deltaTime() { return _frame_time;        };
    float                                       frameRate() { return 1.0f / deltaTime(); };

    glm::vec2                                   mouse()     { return _mouse_position;       };
    glm::vec2                                   dMouse()    { return _delta_mouse_position; };
    bool                                        mouseUp( idk::MouseButton mb );
    bool                                        mouseDown( idk::MouseButton mb );
    void                                        mouseCapture( bool capture );
    bool                                        mouseCaptured();

    uint                                        createGameObject();
    uint                                        createGameObject( uint prefab_id );
    void                                        deleteGameObject( uint obj_id );
    Allocator<int> &                            gameObjects() { return _gameobjects; };
    std::vector<int>                            gameObjects_byComponent( uint component_id );

    void                                        giveComponent( uint obj_id, uint component_id );
    void                                        giveComponents( uint obj_id ) {  }; // Base case
    template <typename... Args> void            giveComponents( uint obj_id,  uint, Args... );
    void                                        removeComponent( uint obj_id, uint component_id );
    bool                                        hasComponent( uint obj_id, uint component_id );

    template <typename module_t> uint           registerCS( std::string name );
    template <typename module_t> module_t &     getCS( uint component_id );
    template <typename module_t> module_t &     getCS( std::string name );
};



template <typename... Args>
void
idk::Engine::giveComponents( uint obj_id, uint first, Args... rest )
{
    giveComponent(obj_id, first);
    giveComponents(obj_id, rest...);
}


template <typename module_t>
uint
idk::Engine::registerCS( std::string name )
{   
    uint cs_id = _idk_componentsystems.size();
    _idk_componentsystems.push_back(new module_t());
    _idk_componentsystems.back()->base_init(_idk_componentsystems.size()-1, name);
    _idk_componentsystems.back()->init(*this);
    _idk_componentsystem_ids[name] = cs_id;
    return cs_id;
}


template <typename module_t>
module_t &
idk::Engine::getCS( uint component_id )
{   
    return *dynamic_cast<module_t *>(_idk_componentsystems[component_id]);
}


template <typename module_t>
module_t &
idk::Engine::getCS( std::string name )
{
    uint cs_id = _idk_componentsystem_ids[name];
    return *dynamic_cast<module_t *>(_idk_componentsystems[cs_id]);
}

