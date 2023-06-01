#pragma once

#include "IDKgraphics/IDKgraphics.h"
#include "gameobject/IDKGameObject.h"


class IDK::Engine
{
private:
    SDL_Event _SDL_event;
    IDK::Keylog _keylog;
    IDK::vec2 _delta_mouse;

    IDK::ptr_Allocator<IDK::GameObject::Base> _gameobject_allocator;

    void _process_key_input();
    void _process_mouse_input();


public:
    IDK::RenderEngine renderer;

    Engine(size_t w = 1000, size_t h = 1000);

    void beginFrame();
    void endFrame();

    IDK::vec2 dMouse() { return _delta_mouse; };

    template <typename gameobject_t>
    gameobject_t *createGameObject();
    void deleteGameObject(int id) { _gameobject_allocator.remove(id); };

};



template <typename gameobject_t>
gameobject_t *IDK::Engine::createGameObject()
{
    int id = _gameobject_allocator.add<gameobject_t>();
    return _gameobject_allocator.get<gameobject_t>(id);
}

