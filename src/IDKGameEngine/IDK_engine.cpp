#include "IDK_engine.h"


idk::Engine::Engine(size_t w, size_t h):
_frametime(1), _render_engine(w, h), _mouse_captured(false)
{

    // v[idk::MouseButton::LEFT, MIDDLE, RIGHT]
    _mouse_down = idk::vector<bool>(3);

}


void
idk::Engine::_process_key_input()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    _keylog.log(state);
}


void
idk::Engine::_reset_mouse_inputs()
{

}


void
idk::Engine::_process_mouse_input()
{
    _reset_mouse_inputs();

    while (SDL_PollEvent(&_SDL_event))
    {
        if (_SDL_event.type == SDL_QUIT)
            exit(0);
    
        if (_SDL_event.type == SDL_MOUSEMOTION)
        {
            _delta_mouse_position.x = _SDL_event.motion.xrel;
            _delta_mouse_position.y = _SDL_event.motion.yrel;
        }

        else if (_SDL_event.type == SDL_MOUSEBUTTONDOWN)
        {
            _mouse_down[_SDL_event.button.button - 1] = true;
        }

        else if (_SDL_event.type == SDL_MOUSEBUTTONUP)
        {
            _mouse_down[_SDL_event.button.button - 1] = false;
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


bool
idk::Engine::mouseUp( idk::MouseButton mb )
{
    return false;
}


bool
idk::Engine::mouseDown( idk::MouseButton mb )
{
    return _mouse_down[static_cast<bool>(mb)];
}


uint
idk::Engine::createGameObject()
{
    uint num_components = _idk_modules.size();
    _gameobject_components.add(std::vector<int>(num_components, 0));

    uint obj_id = _gameobjects.add();
    _gameobjects.get(obj_id).transform_id = _render_engine.createTransform();

    return obj_id;
}


uint
idk::Engine::createGameObject( uint prefab_id )
{
    uint num_components = _idk_modules.size();
    _gameobject_components.add(std::vector<int>(num_components, 0));

    uint obj_id = _gameobjects.add();

    GameObject &obj = _gameobjects.get(obj_id);
    obj.model_id = _gameobjects.get(prefab_id).model_id;
    obj.transform_id = _render_engine.createTransform();

    for (int comp_id = 0; comp_id < _idk_modules.size(); comp_id++)
    {
        if (_gameobject_components.get(prefab_id)[comp_id] == 1)
            giveComponent(obj_id, comp_id);
    }

    return obj_id;
}


idk::Allocator<int> &
idk::Engine::gameObjects_byComponent( uint component_id )
{
    return _gameobjects_by_component[component_id];
}


idk::GameObject &
idk::Engine::getGameObject(uint obj_id)
{
    return _gameobjects.get(obj_id);
}


void
idk::Engine::deleteGameObject( uint obj_id )
{
    for (int i=0; i<_idk_modules.size(); i++)
        removeComponent(obj_id, i);

    rengine().deleteTransform(_gameobjects.get(obj_id).transform_id);

    _gameobjects.remove(obj_id);
    _gameobject_components.remove(obj_id);
}


void
idk::Engine::giveComponent( uint obj_id, uint component_id )
{
    _gameobject_components.get(obj_id)[component_id] = 1;
    _gameobjects_by_component[component_id].add(obj_id, obj_id);
}


void
idk::Engine::removeComponent( uint obj_id, uint component_id )
{
    if (hasComponent(obj_id, component_id) == false)
        return;

    _gameobject_components.get(obj_id)[component_id] = 0;
    _gameobjects_by_component[component_id].remove(obj_id);
}


bool
idk::Engine::hasComponent( uint obj_id, uint component_id )
{
    return _gameobject_components.get(obj_id)[component_id] == 1;
}


void
idk::Engine::translate( uint obj_id, glm::vec3 v )
{
    rengine().getTransform(_gameobjects.get(obj_id).transform_id).translate(v);
}

void
idk::Engine::scale( uint obj_id, glm::vec3 v )
{
    rengine().getTransform(_gameobjects.get(obj_id).transform_id).scale(v);
}


void
idk::Engine::beginFrame()
{
    _frame_start = SDL_GetPerformanceCounter();
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
    _frame_end = SDL_GetPerformanceCounter();
    _frametime = (_frame_end - _frame_start) / (float)SDL_GetPerformanceFrequency();
}



void
idk::Engine::shutdown()
{
    _running = false;
}

