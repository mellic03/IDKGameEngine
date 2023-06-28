#pragma once

#include "IDKGameEngine_common/IDKGameEngine_common.h"
#include "IDK_componentsystem.h"
#include "gameobject/IDK_gameobject.h"


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
    std::vector<std::vector<int>>               _component_matrix; // v[object_id][component_id]
    std::vector<idk::ComponentSystem *>         _idk_componentsystems;


    void                                        _process_key_input();
    void                                        _process_mouse_input();

    void                                        _idk_modules_stage_A();
    void                                        _idk_modules_stage_B();

                                                /* Base case for giveComponents(uint, Args) */
    void                                        giveComponents( uint obj_id ) {  };


public:
                                                Engine( size_t w = 1000, size_t h = 1000 );

    idk::RenderEngine &                         rengine()   { return _render_engine; };
    Keylog &                                    keylog()    { return _keylog; };

    bool                                        running()   { return _running; };
    void                                        beginFrame();
    void                                        endFrame();
    void                                        shutdown();

    float                                       deltaTime() { return _frame_time;         };
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
    template <typename... Args> void            giveComponents( uint obj_id,  uint, Args... );
    void                                        removeComponent( uint obj_id, uint component_id );
    bool                                        hasComponent( uint obj_id, uint component_id );

    template <typename module_t> uint           registerCS( std::string name );
    template <typename module_t> module_t *     getCS( uint component_id );
    template <typename module_t> module_t *     getCS( std::string name );

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
    _idk_componentsystems.push_back(new module_t());
    _idk_componentsystems.back()->base_init(_idk_componentsystems.size()-1, name);
    _idk_componentsystems.back()->init(*this);
    return _idk_componentsystems.size()-1;
}


template <typename module_t>
module_t *
idk::Engine::getCS( uint component_id )
{   
    return dynamic_cast<module_t *>(_idk_componentsystems[component_id]);
}


template <typename module_t>
module_t *
idk::Engine::getCS( std::string name )
{
    for (auto ptr: _idk_componentsystems)
        if (ptr->name() == name)
            return dynamic_cast<module_t *>(ptr);

    return nullptr;
}

