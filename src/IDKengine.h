#pragma once

#include "IDKgraphics/IDKgraphics.h"
#include "gameobject/IDKGameObject.h"


class IDK::Engine
{
private:
    IDK::RenderEngine                           _render_engine;
    
    SDL_Event                                   _SDL_event;
    
    IDK::Keylog                                 _keylog;
    IDK::vec2                                   _delta_mouse;
    IDK::vec2                                   _mouse;

    ptr_Allocator<GameObject::Base>             _gameobject_allocator;

    void                                        _process_key_input();
    void                                        _process_mouse_input();


public:
                                                Engine(size_t w = 1000, size_t h = 1000);

    bool                                        running();
    void                                        beginFrame();
    void                                        endFrame();

    IDK::vec2                                   mouse();
    IDK::vec2                                   dMouse();

    template <typename gameobject_t>
    gameobject_t                                *createGameObject();
    void                                        deleteGameObject(int id);
};


template <typename gameobject_t>
gameobject_t *
IDK::Engine::createGameObject()
{
    int id = _gameobject_allocator.add<gameobject_t>();
    return _gameobject_allocator.get<gameobject_t>(id);
}

