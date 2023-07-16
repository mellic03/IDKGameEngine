#include "idk_engine.h"


idk::Engine::Engine( std::string windowname, size_t w, size_t h ):
_frame_time(1), m_render_engine(windowname, w, h), _mouse_captured(false)
{
    idk::Engine &engine = *this;
    idk::RenderEngine &ren = m_render_engine;
    idk::EventManager &eman = m_event_manager;

    auto resize_lambda = [&ren, &eman]()
    {
        std::cout << "ren.resize()" << std::endl;
        auto windata = eman.windowData();
        ren.resize(windata.width, windata.height);
    };

    auto exit_lambda = [&engine]()
    {
        std::cout << "engine.shutdown()" << std::endl;
        engine.shutdown();
    };

    m_event_manager.onWindowEvent(WindowEvent::RESIZE, resize_lambda);
    m_event_manager.onWindowEvent(WindowEvent::EXIT,   exit_lambda);

}


void
idk::Engine::_idk_CS_stage_A()
{
    for (size_t i=0; i<_idk_componentsystems.size(); i++)
    {
        idk::ComponentSystem *CS = _idk_componentsystems[i];
        CS->stage_A(*this);
    }

    for (size_t i=0; i<_idk_modules.size(); i++)
    {
        idk::Module *mod = _idk_modules[i];
        mod->stage_A(*this);
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

    for (size_t i=0; i<_idk_modules.size(); i++)
    {
        idk::Module *mod = _idk_modules[i];
        mod->stage_B(*this);
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
   
    m_event_manager.processKeyInput();
    m_event_manager.processMouseInput();
    m_event_manager.update();

    _idk_CS_stage_A();

    m_render_engine.beginFrame();
}


void
idk::Engine::endFrame()
{
    m_render_engine.endFrame();
    _idk_CS_stage_B();
    m_render_engine.swapWindow();

    _delta_mouse_position = glm::vec2(0.0f);
    _frame_end = SDL_GetPerformanceCounter();
    _frame_time = ((double)(_frame_end - _frame_start)) / (double)SDL_GetPerformanceFrequency();
}


void
idk::Engine::shutdown()
{
    _running = false;
}

