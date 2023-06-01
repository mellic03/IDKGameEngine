#include "IDKengine.h"


IDK::Engine::Engine(size_t w, size_t h): renderer(w, h)
{
    
}


void IDK::Engine::_process_key_input()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    _keylog.log(state);
}


void IDK::Engine::_process_mouse_input()
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
}


void IDK::Engine::beginFrame()
{
    _process_key_input();
    _process_mouse_input();

    renderer.beginFrame();
}


void IDK::Engine::endFrame()
{

    renderer.endFrame();
}

