#include "IDK_engine.h"


idk::Engine::Engine(size_t w, size_t h):
_frametime(1), _render_engine(w, h), _mouse_captured(false)
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
idk::Engine::_idk_modules_stage_A()
{
    for (size_t i=0; i<_idk_modules.size(); i++)
    {
        idk::Module *idk_module = _idk_modules[i];
        idk_module->stage_A(*this);
    }
}


void
idk::Engine::_idk_modules_stage_B()
{
    for (size_t i=0; i<_idk_modules.size(); i++)
    {
        idk::Module *idk_module = _idk_modules[i];
        idk_module->stage_B(*this);
    }
}


void
idk::Engine::mouseCapture(bool capture)
{
    _mouse_captured = capture;
    SDL_SetRelativeMouseMode((_mouse_captured ? SDL_TRUE : SDL_FALSE));
}


bool
idk::Engine::mouseCaptured()
{
    return _mouse_captured;
}



idk::GameObject &
idk::Engine::createGameObject()
{
    uint num_components = _idk_modules.size();
    uint object_id = _gameobjects.add(GameObject(num_components));

    GameObject &obj = _gameobjects.get(object_id);
    obj.transform_id = _render_engine.createTransform();

    return obj;
}


idk::GameObject &
idk::Engine::getGameObject(uint obj_id)
{
    return _gameobjects.get(obj_id);
}


void
idk::Engine::deleteGameObject(uint obj_id)
{
    _gameobjects.remove(obj_id);
}


void
idk::Engine::beginFrame()
{
    _frame_start = clock();
    _process_key_input();
    _process_mouse_input();
    
    _idk_modules_stage_A();
    
    _render_engine.beginFrame();
}


void
idk::Engine::endFrame()
{
    _render_engine.endFrame();
   
    _idk_modules_stage_B();
    
    _delta_mouse_position = glm::vec2(0.0f);
    _frame_end = clock();
    _frametime = _frame_end - _frame_start;
}



void
idk::Engine::shutdown()
{
    _running = false;
}

