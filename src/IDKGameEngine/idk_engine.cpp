#include "idk_engine.hpp"

#include <libidk/idk_export.hpp>
#include <libidk/idk_scene_file.hpp>

#include <IDKGraphics/IDKGraphics.hpp>
#include <IDKEvents/IDKEvents.hpp>

#include "./idk_engine_api.hpp"
#include "../../external/include/idk_imgui/imgui.hpp"



static idk::EngineAPI *api_ptr;


IDK_VISIBLE
void
idk::Engine::initModules( idk::EngineAPI &api )
{
    api_ptr = &api;

    for (auto *CS: m_componentsystems)
    {
        CS->init(api);
    }

    for (auto *mod: m_idk_modules)
    {
        mod->init(api);
    }

    for (auto &loader: m_module_loaders)
    {
        loader.getInstance()->init(api);
    }
}



void
idk::Engine::_reloadModules()
{
    for (auto &loader: m_module_loaders)
    {
        loader.reload();
        loader.getInstance()->init(*api_ptr);
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

    for (auto &loader: m_module_loaders)
    {
        loader.getInstance()->stage_A(api);
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

    for (auto &loader: m_module_loaders)
    {
        loader.getInstance()->stage_B(api);
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

    for (auto &loader: m_module_loaders)
    {
        loader.getInstance()->stage_C(api);
    }
}



// void
// idk::Engine::idk_CS_onObjectAssignment( int component_id, int obj_id )
// {
//     auto *CS = getCS(component_id);
//     CS->onObjectAssignment(obj_id, *this);
// }



// void
// idk::Engine::idk_CS_onObjectDeassignment( int component_id, int obj_id )
// {
//     auto *CS = getCS(component_id);
//     CS->onObjectDeassignment(obj_id, *this);
// }


// void
// idk::Engine::idk_CS_onObjectCreation( int obj_id )
// {
//     for (auto *CS: m_componentsystems)
//     {
//         CS->onObjectCreation(obj_id, *this);
//     }
// }


// void
// idk::Engine::idk_CS_onObjectDeletion( int obj_id )
// {
//     for (auto *CS: m_componentsystems)
//     {
//         if (hasComponent(obj_id, CS->ID()))
//         {
//             CS->onObjectDeletion(obj_id, *this);
//         }
//     }
// }


// void
// idk::Engine::idk_CS_onObjectCopy( int src_obj_id, int dest_obj_id )
// {
//     for (auto *CS: m_componentsystems)
//     {
//         if (hasComponent(src_obj_id, CS->ID()))
//         {
//             CS->onObjectCopy(src_obj_id, dest_obj_id, *this);
//         }
//     }
// }



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

    if (m_reload)
    {
        _reloadModules();
        m_reload = false;
    }

    m_frame_end = SDL_GetPerformanceCounter();
    m_frame_time = ((double)(m_frame_end - m_frame_start)) / (double)SDL_GetPerformanceFrequency();

    if (m_running == false)
    {
        m_module_loaders.clear();
    }
}


void
idk::Engine::shutdown()
{
    m_running = false;
}


int
idk::Engine::registerModule( const std::string &name, const std::string &filepath )
{
    using ModuleLoader = idk::GenericLoader<idk::Module>;

    int id = m_module_loaders.create(ModuleLoader(filepath));
    m_module_loaders.get(id).getInstance()->preinit(*this);

    std::cout << "Loaded module \"" << name << "\"\n";

    return 0;
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

