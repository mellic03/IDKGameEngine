#include "idk_engine.h"


idk::Engine::Engine( std::string windowname, size_t w, size_t h ):
_frame_time(1), _render_engine(windowname, w, h), _mouse_captured(false)
{
    _mouse_up = std::vector<bool>(3, false);
    _mouse_down = std::vector<bool>(3, false);
}


void
idk::Engine::_process_key_input()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    _keylog.log(state);
}



void
idk::Engine::_process_mouse_input_SDL2()
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

        case (SDL_MOUSEBUTTONDOWN):
            _mouse_down[_SDL_event.button.button - 1] = true;
            _mouse_up[_SDL_event.button.button - 1] = false;
            break;

        case (SDL_MOUSEBUTTONUP):
            _mouse_down[_SDL_event.button.button - 1] = false;
            _mouse_up[_SDL_event.button.button - 1] = true;
            break;
    }}

    int x, y;

    SDL_GetMouseState(&x, &y);
    _mouse_position.x = float(x);
    _mouse_position.y = float(y);

    SDL_GetRelativeMouseState(&x, &y);
    _delta_mouse_position.x = float(x);
    _delta_mouse_position.y = float(y);
}


void
idk::Engine::_process_mouse_input()
{
    #ifdef IDK_SDL2
        _process_mouse_input_SDL2();
    #elif defined(IDK_SFML)
        _process_mouse_input_SFML();
    #endif
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
idk::Engine::_idk_CS_checkDependencies( int obj_id, int component_id )
{
    auto cs = _idk_componentsystems[component_id];
    auto deps = cs->getDependencies();

    for (auto &name: deps)
    {
        std::string assertmsg =
        "object " + std::to_string(obj_id)
        + " does not have component dependency "
        + "\"" + name + "\""
        + " required for component "
        + "\"" + cs->name() + "\"";

        if (hasComponent(obj_id, name) == false)
        {
            std::cout << assertmsg << std::endl;
            exit(1);
        }
    }
}


void
idk::Engine::_idk_CS_onAssignment( int component_id, int obj_id )
{
    #ifdef IDK_DEBUG
        _idk_CS_checkDependencies(obj_id, component_id);
    #endif

    _idk_componentsystems[component_id]->onAssignment(obj_id, *this);
}


void
idk::Engine::_idk_CS_onGameObjectCreation( int obj_id )
{
    for (idk::ComponentSystem *component: _idk_componentsystems)
        component->onGameObjectCreation(obj_id, *this);
}


void
idk::Engine::_idk_CS_onGameObjectDeletion( int obj_id )
{
    for (idk::ComponentSystem *component: _idk_componentsystems)
        component->onGameObjectDeletion(obj_id, *this);
}


void
idk::Engine::_idk_CS_onGameObjectCopy( int src_obj_id, int dest_obj_id )
{
    for (idk::ComponentSystem *component: _idk_componentsystems)
        component->onGameObjectCopy(src_obj_id, dest_obj_id, *this);
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
    return _mouse_up[static_cast<bool>(mb)];
}


bool
idk::Engine::mouseDown( idk::MouseButton mb )
{
    return _mouse_down[static_cast<bool>(mb)];
}


int
idk::Engine::createGameObject()
{
    int num_components = _idk_componentsystems.size();
    _component_matrix.push_back(std::vector<int>(num_components, 0));

    int obj_id = _gameobjects.add();

    _idk_CS_onGameObjectCreation(obj_id);

    return obj_id;
}


int
idk::Engine::createGameObject( int prefab_id )
{
    int num_components = _idk_componentsystems.size();
    _component_matrix.push_back(_component_matrix[prefab_id]);

    int obj_id = _gameobjects.add();

    _idk_CS_onGameObjectCreation(obj_id);
    _idk_CS_onGameObjectCopy(prefab_id, obj_id);

    return obj_id;
}


std::vector<int>
idk::Engine::gameObjects_byComponent( int component_id )
{
    std::vector<int> obj_ids;

    for (int i=0; i<_component_matrix.size(); i++)
        if (_component_matrix[i][component_id] == 1)
            obj_ids.push_back(i);

    return obj_ids;
}


void
idk::Engine::deleteGameObject( int obj_id )
{
    for (int i=0; i<_idk_componentsystems.size(); i++)
        removeComponent(obj_id, i);

    _gameobjects.remove(obj_id);
    _component_matrix[obj_id] = std::vector<int>(_idk_componentsystems.size(), 0);

    _idk_CS_onGameObjectDeletion(obj_id);
}


void
idk::Engine::giveComponent( int obj_id, int component_id )
{
    _component_matrix[obj_id][component_id] = 1;
    _idk_CS_onAssignment(component_id, obj_id);
}


void
idk::Engine::removeComponent( int obj_id, int component_id )
{
    _component_matrix[obj_id][component_id] = 0;
}


bool
idk::Engine::hasComponent( int obj_id, int component_id )
{
    return _component_matrix[obj_id][component_id] == 1;
}


bool 
idk::Engine::hasComponent( int obj_id, std::string component_name )
{
    return _component_matrix[obj_id][_idk_componentsystem_ids[component_name]] == 1;
}


void
idk::Engine::beginFrame()
{
    _frame_start = SDL_GetPerformanceCounter();
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
    _frame_end = SDL_GetPerformanceCounter();
    _frame_time = ((double)(_frame_end - _frame_start)) / (double)SDL_GetPerformanceFrequency();
}


void
idk::Engine::shutdown()
{
    _running = false;
}

