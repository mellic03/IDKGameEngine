#include "idk_engine.h"


idk::ThreadPool idk::Engine::threadpool = idk::ThreadPool(8);


idk::Engine::Engine( std::string name, int w, int h, int res_divisor ):
m_render_engine(name, w, h, res_divisor ), m_threadpool(4)
{
    idk::Engine &engine = *this;
    idk::RenderEngine &ren = m_render_engine;
    idk::EventManager &eman = m_event_manager;

    auto resize_lambda = [&ren, &eman]()
    {
        auto windata = eman.windowData();
        ren.resize(windata.width, windata.height);
    };

    auto exit_lambda = [&engine]()
    {
        engine.shutdown();
    };

    m_event_manager.onWindowEvent(WindowEvent::RESIZE, resize_lambda);
    m_event_manager.onWindowEvent(WindowEvent::EXIT,   exit_lambda);
}


void
idk::Engine::f_idk_CS_stage_A()
{
    idk::Engine *engine = this;

    for (size_t i=0; i<m_idk_componentsystems.size(); i++)
    {
        idk::ComponentSystem *CS = m_idk_componentsystems[i];

        m_threadpool.push(
            [CS, engine]
            {
                CS->stage_A(*engine);
            }
        );
    }

    for (size_t i=0; i<m_idk_modules.size(); i++)
    {
        idk::Module *mod = m_idk_modules[i];
        mod->stage_A(*this);
        // m_threadpool.push(
        //     [mod, engine]
        //     {
        //         mod->stage_A(*engine);
        //     }
        // );
    }

    m_threadpool.join();
}


void
idk::Engine::f_idk_CS_stage_B()
{
    idk::Engine *engine = this;

    for (size_t i=0; i<m_idk_componentsystems.size(); i++)
    {
        idk::ComponentSystem *CS = m_idk_componentsystems[i];

        m_threadpool.push(
            [CS, engine]
            {
                CS->stage_B(*engine);
            }
        );
    }

    for (size_t i=0; i<m_idk_modules.size(); i++)
    {
        idk::Module *mod = m_idk_modules[i];
        mod->stage_B(*this);
        // m_threadpool.push(
        //     [mod, engine]
        //     {
        //         mod->stage_B(*engine);
        //     }
        // );
    }

    m_threadpool.join();
}


void
idk::Engine::f_idk_CS_stage_C()
{
    for (size_t i=0; i<m_idk_componentsystems.size(); i++)
    {
        idk::ComponentSystem *CS = m_idk_componentsystems[i];
        CS->stage_C(*this);
    }

    for (size_t i=0; i<m_idk_modules.size(); i++)
    {
        idk::Module *mod = m_idk_modules[i];
        mod->stage_C(*this);
    }
}


void
idk::Engine::f_idk_CS_checkDependencies( int obj_id, int component_id )
{
    auto cs = m_idk_componentsystems[component_id];
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
idk::Engine::f_idk_CS_onAssignment( int component_id, int obj_id )
{
    #ifdef IDK_DEBUG
        f_idk_CS_checkDependencies(obj_id, component_id);
    #endif

    m_idk_componentsystems[component_id]->onAssignment(obj_id, *this);
}


void
idk::Engine::f_idk_CS_onGameObjectCreation( int obj_id )
{
    for (idk::ComponentSystem *component: m_idk_componentsystems)
        component->onGameObjectCreation(obj_id, *this);
}


void
idk::Engine::f_idk_CS_onGameObjectDeletion( int obj_id )
{
    for (idk::ComponentSystem *component: m_idk_componentsystems)
        component->onGameObjectDeletion(obj_id, *this);
}


void
idk::Engine::f_idk_CS_onGameObjectCopy( int src_obj_id, int dest_obj_id )
{
    for (idk::ComponentSystem *component: m_idk_componentsystems)
        component->onGameObjectCopy(src_obj_id, dest_obj_id, *this);
}


int
idk::Engine::createGameObject()
{
    int num_components = m_idk_componentsystems.size();
    m_component_matrix.push_back(std::vector<int>(num_components, 0));

    int obj_id = m_gameobjects.add();

    f_idk_CS_onGameObjectCreation(obj_id);

    return obj_id;
}


int
idk::Engine::createGameObject( int prefab_id )
{
    int num_components = m_idk_componentsystems.size();
    m_component_matrix.push_back(m_component_matrix[prefab_id]);

    int obj_id = m_gameobjects.add();

    f_idk_CS_onGameObjectCreation(obj_id);
    f_idk_CS_onGameObjectCopy(prefab_id, obj_id);

    return obj_id;
}


std::vector<int>
idk::Engine::gameObjects_byComponent( int component_id )
{
    std::vector<int> obj_ids;

    for (size_t i=0; i<m_component_matrix.size(); i++)
        if (m_component_matrix[i][component_id] == 1)
            obj_ids.push_back(i);

    return obj_ids;
}


void
idk::Engine::deleteGameObject( int obj_id )
{
    for (size_t i=0; i<m_idk_componentsystems.size(); i++)
        removeComponent(obj_id, i);

    m_gameobjects.remove(obj_id);
    m_component_matrix[obj_id] = std::vector<int>(m_idk_componentsystems.size(), 0);

    f_idk_CS_onGameObjectDeletion(obj_id);
}


void
idk::Engine::giveComponent( int obj_id, int component_id )
{
    m_component_matrix[obj_id][component_id] = 1;
    f_idk_CS_onAssignment(component_id, obj_id);
}


void
idk::Engine::removeComponent( int obj_id, int component_id )
{
    m_component_matrix[obj_id][component_id] = 0;
}


bool
idk::Engine::hasComponent( int obj_id, int component_id )
{
    return m_component_matrix[obj_id][component_id] == 1;
}


bool 
idk::Engine::hasComponent( int obj_id, std::string component_name )
{
    return m_component_matrix[obj_id][m_idk_componentsystem_ids[component_name]] == 1;
}


void
idk::Engine::beginFrame()
{
    m_frame_start = SDL_GetPerformanceCounter();
   
    m_event_manager.processKeyInput();
    m_event_manager.processMouseInput();
    m_event_manager.update();


    f_idk_CS_stage_A();

    m_render_engine.beginFrame();
}


void
idk::Engine::endFrame()
{
    m_render_engine.endFrame();
    f_idk_CS_stage_B();
    m_render_engine.swapWindow();
    f_idk_CS_stage_C();

    m_frame_end = SDL_GetPerformanceCounter();
    m_frame_time = ((double)(m_frame_end - m_frame_start)) / (double)SDL_GetPerformanceFrequency();
}


void
idk::Engine::shutdown()
{
    m_running = false;
}

