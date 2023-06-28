#include "IDK_engine.h"


idk::Engine::Engine(size_t w, size_t h):
_frame_time(1), _render_engine(w, h), _mouse_captured(false)
{

    // v[idk::MouseButton::LEFT, MIDDLE, RIGHT]
    _mouse_down = idk::vector<bool>(3, false);

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
    for (size_t i=0; i<_idk_componentsystems.size(); i++)
    {
        idk::ComponentSystem *idk_module = _idk_componentsystems[i];
        idk_module->stage_A(*this);
    }
}


void
idk::Engine::_idk_modules_stage_B()
{
    for (size_t i=0; i<_idk_componentsystems.size(); i++)
    {
        idk::ComponentSystem *idk_module = _idk_componentsystems[i];
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
    uint num_components = _idk_componentsystems.size();
    _component_matrix.push_back(std::vector<int>(num_components, 0));

    uint obj_id = _gameobjects.add();

    for (idk::ComponentSystem *component: _idk_componentsystems)
        component->onGameObjectCreation(obj_id);

    return obj_id;
}


uint
idk::Engine::createGameObject( uint prefab_id )
{
    uint num_components = _idk_componentsystems.size();
    _component_matrix.push_back(_component_matrix[prefab_id]);

    uint obj_id = _gameobjects.add();

    for (idk::ComponentSystem *component: _idk_componentsystems)
    {
        component->onGameObjectCreation(obj_id);
        component->onGameObjectCopy(prefab_id, obj_id);
    }

    return obj_id;
}


std::vector<int>
idk::Engine::gameObjects_byComponent( uint component_id )
{
    std::vector<int> obj_ids;

    for (int i=0; i<_component_matrix.size(); i++)
        if (_component_matrix[i][component_id] == 1)
            obj_ids.push_back(i);

    return obj_ids;
}


void
idk::Engine::deleteGameObject( uint obj_id )
{
    for (int i=0; i<_idk_componentsystems.size(); i++)
        removeComponent(obj_id, i);

    _gameobjects.remove(obj_id);
    _component_matrix[obj_id] = std::vector<int>(_idk_componentsystems.size(), 0);

    for (idk::ComponentSystem *component: _idk_componentsystems)
        component->onGameObjectDeletion(obj_id);
}


void
idk::Engine::giveComponent( uint obj_id, uint component_id )
{
    _component_matrix[obj_id][component_id] = 1;
    _idk_componentsystems[component_id]->onAssignment(obj_id, *this);
}


void
idk::Engine::removeComponent( uint obj_id, uint component_id )
{
    _component_matrix[obj_id][component_id] = 0;
}


bool
idk::Engine::hasComponent( uint obj_id, uint component_id )
{
    return _component_matrix[obj_id][component_id] == 1;
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
    _frame_time = (_frame_end - _frame_start) / (float)SDL_GetPerformanceFrequency();
}



void
idk::Engine::shutdown()
{
    _running = false;
}

