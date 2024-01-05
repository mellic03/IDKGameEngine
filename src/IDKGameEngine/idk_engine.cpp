#include "idk_engine.hpp"
#include <libidk/idk_export.hpp>

#include <IDKRenderEngine/render/idk_renderengine.hpp>
#include "../IDKBuiltinCS/idk_name_cs.hpp"



idk::Engine::Engine( idk::RenderEngine &ren )
{
    idk::Engine       &engine = *this;
    idk::EventManager &events = m_event_manager;

    auto resize_lambda = [&ren, &events]()
    {
        auto winsize = events.windowSize();
        ren.resize(winsize.x, winsize.y);
    };

    auto exit_lambda = [&engine]()
    {
        engine.shutdown();
    };


    m_event_manager.onDropFile(
        []( const char *filepath )
        {
            std::cout << "filepath: " << filepath << "\n";
        }
    );

    m_event_manager.onWindowEvent(WindowEvent::RESIZE, resize_lambda);
    m_event_manager.onWindowEvent(WindowEvent::EXIT,   exit_lambda);
}



IDK_VISIBLE
void
idk::Engine::_idk_modules_init()
{
    for (auto &CS: m_componentsystems)
    {
        CS->init(*APIptr);
    }

    for (auto &mod: m_idk_modules)
    {
        mod->init(*APIptr);
    }
}



void
idk::Engine::_idk_modules_stage_A()
{
    for (auto &CS: m_componentsystems)
    {
        CS->stage_A(*APIptr);
    }

    for (auto &mod: m_idk_modules)
    {
        mod->stage_A(*APIptr);
    }
}


void
idk::Engine::_idk_modules_stage_B()
{
    for (auto &CS: m_componentsystems)
    {
        CS->stage_B(*APIptr);
    }

    for (auto &mod: m_idk_modules)
    {
        mod->stage_B(*APIptr);
    }
}


void
idk::Engine::_idk_modules_stage_C()
{
    for (auto &CS: m_componentsystems)
    {
        CS->stage_C(*APIptr);
    }

    for (auto &mod: m_idk_modules)
    {
        mod->stage_C(*APIptr);
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
        if (hasComponent(obj_id, CS->ID()))
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
        if (hasComponent(src_obj_id, CS->ID()))
        {
            CS->onObjectCopy(src_obj_id, dest_obj_id, *this);
        }
    }
}


int
idk::Engine::createGameObject( const std::string &name )
{
    int obj_id = m_gameobjects.create();
    m_gameobjects.get(obj_id) = obj_id;

    idk_CS_onObjectCreation(obj_id);
    getCS<idk::Name_CS>().setName(obj_id, name);

    return obj_id;
}


int
idk::Engine::copyGameObject( int prefab_id, const std::string &name )
{
    int obj_id = createGameObject(name);
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


IDK_VISIBLE
void
idk::Engine::_begin_frame( idk::RenderEngine &ren )
{
    m_frame_start = SDL_GetPerformanceCounter();
   
    m_event_manager.processKeyInput();
    m_event_manager.processMouseInput();
    m_event_manager.update();

    ren.beginFrame();
    this->_idk_modules_stage_A();
}


IDK_VISIBLE
void
idk::Engine::_end_frame( idk::RenderEngine &ren )
{
    ren.endFrame(deltaTime());
    _idk_modules_stage_B();
    ren.swapWindow();
    _idk_modules_stage_C();

    m_frame_end = SDL_GetPerformanceCounter();
    m_frame_time = ((double)(m_frame_end - m_frame_start)) / (double)SDL_GetPerformanceFrequency();
}


void
idk::Engine::shutdown()
{
    m_running = false;
}


int
idk::Engine::registerModule( const std::string &name, const std::string &filepath )
{
    idk::APILoader loader("IDKGE/runtime/libIDKEditorUI");
    idk::Module *module_ptr = loader.call<idk::Module>("getInstance");

    int module_id = m_idk_modules.create(module_ptr);

    m_idk_modules.get(module_id)->base_init(module_id, name);
    m_idk_module_ids[name] = module_id;

    return module_id;

}
