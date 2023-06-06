#pragma once

#include "../IDKgraphics/IDKgraphics.h"
#include "IDKGameEngine_common/IDKGameEngine_common.h"
#include "gameobject/IDKGameObject.h"


class idk::Engine
{
private:
    idk::RenderEngine                           _render_engine;
    
    SDL_Event                                   _SDL_event;

    idk::Keylog                                 _keylog;
    glm::vec2                                   _delta_mouse_position;
    glm::vec2                                   _mouse_position;

    ptr_Allocator<GameObject::Base>             _gameobject_allocator;
    idk::vector<idk::Module *>                  _idk_modules;

    void                                        _process_key_input();
    void                                        _process_mouse_input();


public:
                                                Engine(size_t w = 1000, size_t h = 1000);

    idk::RenderEngine &                         rengine()       { return _render_engine; };

    bool                                        running()       { return true; };
    void                                        beginFrame();
    void                                        endFrame();

    glm::vec2                                   mouse()         { return _mouse_position; };
    glm::vec2                                   dMouse()        { return _delta_mouse_position; };

    template <typename gameobject_t>
    gameobject_t *                              createGameObject();
    void                                        deleteGameObject(int id);

    template <typename idk_module_t>
    void                                        registerModule() { _idk_modules.push(new idk_module_t()); };
};


template <typename gameobject_t>
gameobject_t *
idk::Engine::createGameObject()
{
    int id = _gameobject_allocator.add<gameobject_t>();
    return _gameobject_allocator.get<gameobject_t>(id);
}

