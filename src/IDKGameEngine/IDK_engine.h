#pragma once

#include "../IDKgraphics/IDKgraphics.h"
#include "IDKGameEngine_common/IDKGameEngine_common.h"
#include "gameobject/IDK_gameobject.h"


class idk::Engine
{
private:    
    idk::RenderEngine                           _render_engine;
    
    SDL_Event                                   _SDL_event;

    idk::Keylog                                 _keylog;
    glm::vec2                                   _delta_mouse_position;
    glm::vec2                                   _mouse_position;

    idk::Allocator<GameObject>                  _gameobjects;


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

    uint                                        createGameObject();
    idk::GameObject &                           getGameObject(uint obj_id);
    void                                        deleteGameObject(uint obj_id);
    Allocator<GameObject> &                     gameObjects() { return _gameobjects; };

    template <typename idk_module_t>
    void                                        registerModule() { _idk_modules.push(new idk_module_t()); };
};
