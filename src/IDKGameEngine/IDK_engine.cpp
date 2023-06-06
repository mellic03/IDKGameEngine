#include "IDK_engine.h"


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
            _delta_mouse_position.x = _SDL_event.motion.xrel;
            _delta_mouse_position.y = _SDL_event.motion.yrel;
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
    _mouse_position.x = float(x);
    _mouse_position.y = float(y);
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

    for (idk::Module *module: _idk_modules)
    {
        module->beginFrame(*this);
    }
}


void
idk::Engine::endFrame()
{

    for (idk::Module *module: _idk_modules)
    {
        module->endFrame(*this);
    }
    
    _render_engine.endFrame();
}

