#pragma once

#include "../IDKgraphics/IDKgraphics.h"
#include "IDKGameEngine_common/IDKGameEngine_common.h"
#include "gameobject/IDK_gameobject.h"
#include "IDK_controller.h"


class idk::Engine
{
private:
    clock_t                                     _frame_start;
    clock_t                                     _frame_end;
    clock_t                                     _frametime;

    bool                                        _running        = true;

    idk::RenderEngine                           _render_engine;
    
    SDL_Event                                   _SDL_event;

    idk::Keylog                                 _keylog;
    glm::vec2                                   _delta_mouse_position;
    glm::vec2                                   _mouse_position;
    bool                                        _mouse_captured;

    idk::Allocator<GameObject>                  _gameobjects;
    std::vector<idk::Module *>                  _idk_modules;

    void                                        _process_key_input();
    void                                        _process_mouse_input();

    void                                        _idk_modules_stage_A();
    void                                        _idk_modules_stage_B();
    void                                        _idk_modules_stage_C();

public:
                                                Engine(size_t w = 1000, size_t h = 1000);

    idk::RenderEngine &                         rengine()       { return _render_engine; };

    bool                                        running()       { return _running; };
    void                                        beginFrame();
    void                                        endFrame();
    void                                        shutdown();

    float                                       deltaTime()  { return ((float)_frametime) / CLOCKS_PER_SEC; };
    float                                       frameRate()  { return 1.0f / deltaTime();                   };

    glm::vec2                                   mouse()         { return _mouse_position; };
    glm::vec2                                   dMouse()        { return _delta_mouse_position; };
    void                                        mouseCapture(bool on);
    bool                                        mouseCaptured();


    Keylog &                                    keylog()    { return _keylog; };

    idk::GameObject &                           createGameObject();
    idk::GameObject &                           getGameObject(uint obj_id);
    void                                        deleteGameObject(uint obj_id);
    Allocator<GameObject> &                     gameObjects() { return _gameobjects; };

    template <typename module_t>
    void                                        registerModule();

};


template <typename module_t>
void
idk::Engine::registerModule()
{   
    _idk_modules.push_back(new module_t);
    _idk_modules.back()->init(*this);
}

