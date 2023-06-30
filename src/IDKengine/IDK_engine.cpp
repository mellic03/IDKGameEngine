#include "IDK_engine.h"


idk::Engine::Engine(size_t w, size_t h):
_frame_time(1), _render_engine(w, h), _mouse_captured(false)
{
    _mouse_down = idk::vector<bool>(3, false); // { LEFT, MIDDLE, RIGHT }

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
    while (SDL_PollEvent(&_SDL_event)) {
    switch (_SDL_event.type)
    {
        case (SDL_QUIT):
            _running = false;
            break;

        case SDL_WINDOWEVENT:
            if (_SDL_event.window.event == SDL_WINDOWEVENT_CLOSE)
                _running = false;
            else if (_SDL_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                _render_engine.resize(_SDL_event.window.data1, _SDL_event.window.data2);
            break;

        case (SDL_MOUSEMOTION):
            _delta_mouse_position.x = _SDL_event.motion.xrel;
            _delta_mouse_position.y = _SDL_event.motion.yrel;
            break;
        
        case (SDL_MOUSEBUTTONDOWN):
            _mouse_down[_SDL_event.button.button - 1] = true;
            break;

        case (SDL_MOUSEBUTTONUP):
            _mouse_down[_SDL_event.button.button - 1] = false;
            break;
    }}


    int x, y;
    SDL_GetMouseState(&x, &y);

    _mouse_position.x = float(x);
    _mouse_position.y = float(y);
}


void
idk::Engine::_idk_CS_stage_A()
{
    for (size_t i=0; i<_idk_componentsystems.size(); i++)
    {
        idk::ComponentSystem *CS = _idk_componentsystems[i];
        CS->stage_A(*this);
    }
}


void
idk::Engine::_idk_CS_stage_B()
{
    for (size_t i=0; i<_idk_componentsystems.size(); i++)
    {
        idk::ComponentSystem *CS = _idk_componentsystems[i];
        CS->stage_B(*this);
    }
}


void
idk::Engine::_idk_CS_onCreation( int obj_id )
{
    for (idk::ComponentSystem *component: _idk_componentsystems)
        component->onGameObjectCreation(obj_id);
}


void
idk::Engine::_idk_CS_onDeletion( int obj_id )
{
    for (idk::ComponentSystem *component: _idk_componentsystems)
        component->onGameObjectDeletion(obj_id);
}


void
idk::Engine::_idk_CS_onCopy( int src_obj_id, int dest_obj_id )
{
    for (idk::ComponentSystem *component: _idk_componentsystems)
        component->onGameObjectCopy(src_obj_id, dest_obj_id);
}


void
idk::Engine::_idk_CS_onAssignment( int component_id, int obj_id )
{
    _idk_componentsystems[component_id]->onAssignment(obj_id, *this);
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

    _idk_CS_onCreation(obj_id);

    return obj_id;
}


uint
idk::Engine::createGameObject( uint prefab_id )
{
    uint num_components = _idk_componentsystems.size();
    _component_matrix.push_back(_component_matrix[prefab_id]);

    uint obj_id = _gameobjects.add();

    _idk_CS_onCreation(obj_id);
    _idk_CS_onCopy(prefab_id, obj_id);

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

    _idk_CS_onDeletion(obj_id);
}


void
idk::Engine::giveComponent( uint obj_id, uint component_id )
{
    _component_matrix[obj_id][component_id] = 1;
    _idk_CS_onAssignment(component_id, obj_id);
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
    _frame_start = SDL_GetTicks64();
    _process_key_input();
    _process_mouse_input();
    
    _idk_CS_stage_A();

    _render_engine.beginFrame();
}


void
idk::Engine::endFrame()
{
    _render_engine.endFrame();
   
    _idk_CS_stage_B();
    
    _delta_mouse_position = glm::vec2(0.0f);
    _frame_end = SDL_GetTicks64();
    _frame_time = (_frame_end - _frame_start) / 1000.0f;
}


void
idk::Engine::shutdown()
{
    _running = false;
}

