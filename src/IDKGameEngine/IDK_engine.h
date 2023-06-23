#pragma once

#include "IDKGameEngine_common/IDKGameEngine_common.h"
#include "IDK_module.h"
#include "gameobject/IDK_gameobject.h"



class ComponentManager
{
private:
    std::vector<std::vector<int>>   _obj_has_cmp;

public:


};


class idk::Engine
{
private:
    Uint64                                      _frame_start;
    Uint64                                      _frame_end;
    float                                       _frametime;
    bool                                        _running = true;

    idk::RenderEngine                           _render_engine;
    SDL_Event                                   _SDL_event;
    idk::Keylog                                 _keylog;
    idk::vector<bool>                           _mouse_down;


    glm::vec2                                   _delta_mouse_position;
    glm::vec2                                   _mouse_position;
    bool                                        _mouse_captured;

    idk::Allocator<GameObject>                  _gameobjects;
    idk::Allocator<std::vector<int>>            _gameobject_components;     // v.get(obj) = { components }
    std::vector<std::vector<int>>               _has_component; // v[object_id][component_id]
    std::vector<idk::Module *>                  _idk_modules;

    void                                        _process_key_input();
    void                                        _reset_mouse_inputs();
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

    float                                       deltaTime() { return _frametime;         };
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
    idk::GameObject &                           getGameObject( uint obj_id );
    Allocator<GameObject> &                     gameObjects() { return _gameobjects; };
    std::vector<int>                            gameObjects_byComponent( uint component_id );

    void                                        giveComponent( uint obj_id, uint component_id );
    void                                        removeComponent( uint obj_id, uint component_id );
    bool                                        hasComponent( uint obj_id, uint component_id );

    void                                        translate( uint obj_id, glm::vec3 v );
    void                                        scale( uint obj_id, glm::vec3 v );

    template <typename module_t>
    module_t *                                  registerModule( std::string name );

    template <typename module_t>
    module_t *                                  getModule( std::string name );

};


template <typename module_t>
module_t *
idk::Engine::registerModule( std::string name )
{   
    _idk_modules.push_back(new module_t(name, _idk_modules.size()));
    _idk_modules.back()->init(*this);
    return dynamic_cast<module_t *>(_idk_modules.back());
}


template <typename module_t>
module_t *
idk::Engine::getModule( std::string name )
{
    module_t *ptr;

    for (idk::Module *moduleptr: _idk_modules)
    {

    }

    return ptr;
}

