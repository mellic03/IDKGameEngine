#include "idk_engine.hpp"

#include <libidk/idk_export.hpp>
#include <libidk/idk_scene_file.hpp>

#include <IDKGraphics/IDKGraphics.hpp>
#include <IDKEvents/IDKEvents.hpp>

#include "./idk_engine_api.hpp"


idk::Engine::Engine()
{
    // idk::Engine &engine = *this;

    // auto resize_lambda = [ren, eventsys]()
    // {
    //     auto winsize = eventsys->windowSize();
    //     ren->resize(winsize.x, winsize.y);
    // };

    // auto exit_lambda = [&engine]()
    // {
    //     engine.shutdown();
    // };

    // eventsys->onWindowEvent(WindowEvent::RESIZE, resize_lambda);
    // eventsys->onWindowEvent(WindowEvent::EXIT,   exit_lambda);
}



IDK_VISIBLE
void
idk::Engine::initModules( idk::EngineAPI &api )
{
    for (auto *CS: m_componentsystems)
    {
        CS->init(api);
    }

    for (auto *mod: m_idk_modules)
    {
        mod->init(api);
    }
}



void
idk::Engine::_idk_modules_stage_A( idk::EngineAPI &api )
{
    for (auto *CS: m_componentsystems)
    {
        CS->stage_A(api);
    }

    for (auto *mod: m_idk_modules)
    {
        mod->stage_A(api);
    }
}


void
idk::Engine::_idk_modules_stage_B( idk::EngineAPI &api )
{
    for (auto *CS: m_componentsystems)
    {
        CS->stage_B(api);
    }

    for (auto *mod: m_idk_modules)
    {
        mod->stage_B(api);
    }
}


void
idk::Engine::_idk_modules_stage_C( idk::EngineAPI &api )
{
    for (auto *CS: m_componentsystems)
    {
        CS->stage_C(api);
    }

    for (auto *mod: m_idk_modules)
    {
        mod->stage_C(api);
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
//     #ifdef IDK_DEBUG
//         idk_CS_checkDependencies(obj_id, component_id);
//     #endif

    auto *CS = getCS(component_id);
    CS->onObjectAssignment(obj_id, *this);
}


void
idk::Engine::idk_CS_onObjectDeassignment( int component_id, int obj_id )
{
    auto *CS = getCS(component_id);
    CS->onObjectDeassignment(obj_id, *this);
}


void
idk::Engine::idk_CS_onObjectCreation( int obj_id )
{
    for (auto *CS: m_componentsystems)
    {
        CS->onObjectCreation(obj_id, *this);
    }
}


void
idk::Engine::idk_CS_onObjectDeletion( int obj_id )
{
    for (auto *CS: m_componentsystems)
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
    for (auto *CS: m_componentsystems)
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
                 m_gameobject_names.create(name);

    m_gameobjects.get(obj_id) = obj_id;
    idk_CS_onObjectCreation(obj_id);

    return obj_id;
}


int
idk::Engine::copyGameObject( int prefab_id )
{
    int obj_id = createGameObject(getGameObjectName(prefab_id));
    idk_CS_onObjectCopy(prefab_id, obj_id);

    return obj_id;
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
    m_gameobject_names.destroy(obj_id);
}


std::string &
idk::Engine::getGameObjectName( int obj_id )
{
    return m_gameobject_names.get(obj_id);
}


void
idk::Engine::setGameObjectName( int obj_id, const std::string &name )
{
    m_gameobject_names.get(obj_id) = name;
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
    idk_CS_onObjectDeassignment(component_id, obj_id);
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
idk::Engine::beginFrame( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();

    m_frame_start = SDL_GetPerformanceCounter();
   
    ren.beginFrame();
    this->_idk_modules_stage_A(api);
}


IDK_VISIBLE
void
idk::Engine::endFrame( idk::EngineAPI &api )
{
    auto &ren = api.getRenderer();

    ren.endFrame(deltaTime());
    _idk_modules_stage_B(api);
    ren.swapWindow();
    _idk_modules_stage_C(api);

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
    idk::APILoader loader(filepath.c_str());
    idk::Module *module_ptr = loader.call<idk::Module>("getInstance");

    int module_id = m_idk_modules.create(module_ptr);

    m_idk_modules.get(module_id)->base_init(module_id, name);
    m_idk_module_ids[name] = module_id;

    std::cout << "Loaded module \"" << name << "\"\n";

    return module_id;

}


idk::ComponentSystem *
idk::Engine::getCS( int component_id )
{
    return m_componentsystems.get(component_id);
};



// void
// idk::Engine::saveFile( const std::string &filepath )
// {
//     std::ofstream stream(filepath, std::ios::binary);
//     idk::SceneFile scenefile;

//     for (auto *CS: getComponentSystems())
//     {
//         scenefile.CSFiles.push_back(
//             CS->onFileSave(*this)
//         );
//     }

//     stream.close();
// }



// void
// idk::Engine::loadFile( const std::string &filepath )
// {
//     std::ifstream stream(filepath, std::ios::binary);

//     idk::SceneFile scenefile = idk::Scene::loadFile(filepath.c_str());
//     auto &CSFiles = scenefile.CSFiles;
//     std::reverse(CSFiles.begin(), CSFiles.end());

//     for (auto *CS: getComponentSystems())
//     {
//         CS->onFileLoad(*this, CSFiles.back());
//         CSFiles.pop_back();
//     }

//     stream.close();
// }

