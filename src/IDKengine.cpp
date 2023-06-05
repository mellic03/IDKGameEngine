#include "IDKengine.h"


idk::Engine::Engine(size_t w, size_t h): _render_engine(w, h)
{
    
}


void
idk::Engine::_process_key_input()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    _keylog.log(state);
}


void
idk::Engine::_process_mouse_input()
{
    while (SDL_PollEvent(&_SDL_event))
    {
        if (_SDL_event.type == SDL_QUIT)
            exit(0);
    
        if (_SDL_event.type == SDL_MOUSEMOTION)
        {
            _delta_mouse.x = _SDL_event.motion.xrel;
            _delta_mouse.y = _SDL_event.motion.yrel;
        }
    }

    switch (_SDL_event.type)
    {
        case SDL_WINDOWEVENT:
            if (_SDL_event.window.event == SDL_WINDOWEVENT_CLOSE)
                exit(1);
            break;
    }

    int x, y;
    SDL_GetMouseState(&x, &y);
    _mouse.x = float(x);
    _mouse.y = float(y);
}


bool
idk::Engine::running()
{
    return true;
}


idk::vec2
idk::Engine::mouse()
{
    return _mouse;
}


idk::vec2
idk::Engine::dMouse()
{
    return _delta_mouse;
}


void
idk::Engine::deleteGameObject(int id)
{
    _gameobject_allocator.remove(id);
}


void
idk::Engine::beginFrame()
{
    _process_key_input();
    _process_mouse_input();

    _render_engine.beginFrame();
}


void
idk::Engine::endFrame()
{

    _render_engine.endFrame();
}

