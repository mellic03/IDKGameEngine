#include "idk_engine.hpp"


// idk::ThreadPool idk::Engine::threadpool = idk::ThreadPool(8);


idk::Engine::Engine( std::string name, int w, int h, int res_divisor )
{
    m_render_engine.init(name, w, h);

    idk::Engine &engine = *this;
    idk::RenderEngine &ren = m_render_engine;
    idk::EventManager &eman = m_event_manager;

    auto resize_lambda = [&ren, &eman]()
    {
        auto winsize = eman.windowSize();
        ren.resize(winsize.x, winsize.y);
    };

    auto exit_lambda = [&engine]()
    {
        engine.shutdown();
    };

    m_event_manager.onWindowEvent(WindowEvent::RESIZE, resize_lambda);
    m_event_manager.onWindowEvent(WindowEvent::EXIT,   exit_lambda);
}


void
idk::Engine::_idk_modules_init()
{
    for (auto &CS: m_componentsystems)
    {
        CS->init(*this);
    }

    for (auto &mod: m_idk_modules)
    {
        mod->init(*this);
    }
}



void
idk::Engine::_idk_modules_stage_A()
{
    for (auto &CS: m_componentsystems)
    {
        CS->stage_A(*this);
    }

    for (auto &mod: m_idk_modules)
    {
        mod->stage_A(*this);
    }
}


void
idk::Engine::_idk_modules_stage_B()
{
    for (auto &CS: m_componentsystems)
    {
        CS->stage_B(*this);
    }

    for (auto &mod: m_idk_modules)
    {
        mod->stage_B(*this);
    }
}


void
idk::Engine::_idk_modules_stage_C()
{
    for (auto &CS: m_componentsystems)
    {
        CS->stage_C(*this);
    }

    for (auto &mod: m_idk_modules)
    {
        mod->stage_C(*this);
    }
}


void
idk::Engine::idk_CS_checkDependencies( int obj_id, int component_id )
{
    auto cs = m_componentsystems.get(component_id);
    auto deps = cs->getDependencies();

    // for (auto &name: deps)
    // {
    //     std::string assertmsg =
    //     "object " + std::to_string(obj_id)
    //     + " does not have component dependency "
    //     + "\"" + name + "\""
    //     + " required for component "
    //     + "\"" + cs->name() + "\"";

    //     if (hasComponent(obj_id, name) == false)
    //     {
    //         std::cout << assertmsg << std::endl;
    //         exit(1);
    //     }
    // }
}


void
idk::Engine::idk_CS_onObjectAssignment( int component_id, int obj_id )
{
    #ifdef IDK_DEBUG
        idk_CS_checkDependencies(obj_id, component_id);
    #endif

    m_componentsystems.get(component_id)->onObjectAssignment(obj_id, *this);
}


void
idk::Engine::idk_CS_onObjectCreation( int obj_id )
{
    for (auto &CS: m_componentsystems)
    {
        CS->onObjectCreation(obj_id, *this);
    }
}


void
idk::Engine::idk_CS_onObjectDeletion( int obj_id )
{
    for (auto &CS: m_componentsystems)
    {
        if (hasComponent(obj_id, CS->id()))
        {
            CS->onObjectDeletion(obj_id, *this);
        }
    }
}


void
idk::Engine::idk_CS_onObjectCopy( int src_obj_id, int dest_obj_id )
{
    for (auto &CS: m_componentsystems)
    {
        if (hasComponent(src_obj_id, CS->id()))
        {
            CS->onObjectCopy(src_obj_id, dest_obj_id, *this);
        }
    }
}


int
idk::Engine::createGameObject()
{
    int obj_id = m_gameobjects.create();
    m_gameobjects.get(obj_id) = obj_id;

    idk_CS_onObjectCreation(obj_id);

    return obj_id;
}


int
idk::Engine::createGameObject( int prefab_id )
{
    int obj_id = createGameObject();
    m_gameobjects.get(obj_id) = obj_id;

    idk_CS_onObjectCopy(prefab_id, obj_id);

    return obj_id;
}



const std::set<int> &
idk::Engine::gameObjects_byComponent( int component_id )
{
    return m_components_objects[component_id];
}


void
idk::Engine::deleteGameObject( int obj_id )
{
    idk_CS_onObjectDeletion(obj_id);

    m_objects_components[obj_id].clear();

    for (auto &[cs_id, object_ids]: m_components_objects)
    {
        object_ids.erase(obj_id);
    }

    m_gameobjects.destroy(obj_id);
}


void
idk::Engine::giveComponent( int obj_id, int component_id )
{
    m_objects_components[obj_id].insert(component_id);
    m_components_objects[component_id].insert(obj_id);
    idk_CS_onObjectAssignment(component_id, obj_id);
}


void
idk::Engine::removeComponent( int obj_id, int component_id )
{
    m_objects_components[obj_id].erase(component_id);
    m_components_objects[component_id].erase(obj_id);
}


bool
idk::Engine::hasComponent( int obj_id, int component_id )
{
    return m_objects_components[obj_id].find(component_id) != m_objects_components[obj_id].end();
}


void
idk::Engine::beginFrame()
{
    m_frame_start = SDL_GetPerformanceCounter();
   
    m_event_manager.processKeyInput();
    m_event_manager.processMouseInput();
    m_event_manager.update();

    m_render_engine.beginFrame();
    this->_idk_modules_stage_A();
}


void
idk::Engine::endFrame()
{
    m_render_engine.endFrame(deltaTime());
    _idk_modules_stage_B();
    m_render_engine.swapWindow();
    _idk_modules_stage_C();

    m_frame_end = SDL_GetPerformanceCounter();
    m_frame_time = ((double)(m_frame_end - m_frame_start)) / (double)SDL_GetPerformanceFrequency();
}


void
idk::Engine::shutdown()
{
    m_running = false;

    // idk::Engine::threadpool.stop();
}

