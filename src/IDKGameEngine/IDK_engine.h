#pragma once

#include "IDKGameEngine_common/IDKGameEngine_common.h"
#include "IDK_module.h"
#include "gameobject/IDK_gameobject.h"



class idk::Engine
{
private:
    clock_t                                     _frame_start;
    clock_t                                     _frame_end;
    clock_t                                     _frametime;
    bool                                        _running = true;

    idk::RenderEngine                           _render_engine;
    SDL_Event                                   _SDL_event;
    idk::Keylog                                 _keylog;

    glm::vec2                                   _delta_mouse_position;
    glm::vec2                                   _mouse_position;
    bool                                        _mouse_captured;

    idk::Allocator<GameObject>                  _gameobjects;
    std::vector<std::vector<int>>               _gameobject_components;
    std::vector<idk::Allocator<int>>            _gameobjects_by_component;
    idk::Allocator<GameObject>                  _prefabs;
    std::vector<idk::Module *>                  _idk_modules;

    void                                        _process_key_input();
    void                                        _process_mouse_input();

    void                                        _idk_modules_stage_A();
    void                                        _idk_modules_stage_B();

public:
                                                Engine( size_t w = 1000, size_t h = 1000 );

    idk::RenderEngine &                         rengine()   { return _render_engine; };
    Keylog &                                    keylog()    { return _keylog; };

    bool                                        running()   { return _running; };
    void                                        beginFrame();
    void                                        endFrame();
    void                                        shutdown();

    float                                       deltaTime() { return ((float)_frametime) / CLOCKS_PER_SEC; };
    float                                       frameRate() { return 1.0f / deltaTime();                   };

    glm::vec2                                   mouse()     { return _mouse_position; };
    glm::vec2                                   dMouse()    { return _delta_mouse_position; };
    void                                        mouseCapture( bool capture );
    bool                                        mouseCaptured();

    uint                                        createPrefab( uint obj_id );
    uint                                        createGameObject();
    uint                                        createGameObject( uint prefab_id );
    void                                        deleteGameObject( uint obj_id );
    idk::GameObject &                           getGameObject( uint obj_id );
    Allocator<GameObject> &                     gameObjects() { return _gameobjects; };
    Allocator<int> &                            gameObjects_byComponent( uint component_id );

    void                                        giveComponent( uint object_id, uint component_id );
    void                                        removeComponent( uint object_id, uint component_id );
    bool                                        hasComponent( uint object_id, uint component_id );
    void                                        translate( uint obj_id, glm::vec3 v);
    void                                        scale( uint obj_id, glm::vec3 v);


    template <typename module_t>
    module_t *                                  registerModule();

};


template <typename module_t>
module_t *
idk::Engine::registerModule()
{   
    _gameobjects_by_component.push_back(idk::Allocator<int>());

    _idk_modules.push_back(new module_t(_idk_modules.size()));
    _idk_modules.back()->init(*this);
    return dynamic_cast<module_t *>(_idk_modules.back());
}

