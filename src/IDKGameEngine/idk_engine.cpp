#include "idk_engine.hpp"

#include <libidk/idk_export.hpp>
#include <libidk/idk_platform.hpp>
#include <libidk/idk_scene_file.hpp>
#include <libidk/idk_log.hpp>

#include <IDKGraphics/IDKGraphics.hpp>
#include <IDKEvents/IDKEvents.hpp>

#include "./idk_engine_api.hpp"
#include "../../external/include/idk_imgui/imgui.hpp"

#include <filesystem>


// static idk::EngineAPI *api_ptr;


// IDK_VISIBLE
// void
// idk::Engine::initModules( idk::EngineAPI &api )
// {
//     api_ptr = &api;

//     for (auto &loader: m_modules)
//     {
//         loader.getInstance()->init(api);
//     }
// }



void
idk::Engine::_idk_modules_stage_A( idk::EngineAPI &api )
{
    for (auto &[key, loader]: m_modules)
    {
        loader.getInstance()->stage_A(api);
    }
}


void
idk::Engine::_idk_modules_stage_B( idk::EngineAPI &api )
{
    for (auto &[key, loader]: m_modules)
    {
        loader.getInstance()->stage_B(api);
    }
}


void
idk::Engine::_idk_modules_stage_C( idk::EngineAPI &api )
{
    for (auto &[key, loader]: m_modules)
    {
        loader.getInstance()->stage_C(api);
    }
}



IDK_VISIBLE
void
idk::Engine::beginFrame( idk::EngineAPI &api, float dt )
{
    static std::vector<int> cull;
    cull.clear();

    for (auto &[id, loader]: m_modules)
    {
        if (loader.should_unload)
        {
            LOG_INFO("Reloading module with ID {}", id);

            cull.push_back(id);
        }

        if (loader.should_reload)
        {
            LOG_INFO("Reloading module with ID {}", id);

            loader.reload();
            loader.getInstance()->init(*m_api);
            loader.onReload();
            loader.should_reload = false;
        }
    }

    for (int id: cull)
    {
        m_modules.get(id).unload();
        m_modules.destroy(id);
    }

    auto &ren = api.getRenderer();
    m_frame_time = dt;

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
    api.getWindow().swapWindow();
    _idk_modules_stage_C(api);
}


void
idk::Engine::shutdown()
{
    // LOG_INFO("idk::Engine::shutdown");
    LOG_INFO("");
    m_running = false;
}



int
idk::Engine::loadModule( const std::string &filename )
{
    using ModuleLoader = idk::GenericLoader<idk::Module>;
    namespace fs = std::filesystem;

    int id = m_modules.create(ModuleLoader(filename + IDK_DLIB_EXT));
    m_modules.get(id).getInstance()->init(*m_api);

    LOG_INFO("Loaded module from {} with id {}", filename, id);

    return id;
}


void
idk::Engine::unloadModule( int id )
{
    m_modules.get(id).should_unload = true;

    LOG_INFO("Schedule unload for module with ID {}", id);
}



void
idk::Engine::reloadModule( int id, const std::function<void()> &callback )
{
    m_modules.get(id).should_reload = true;
    m_modules.get(id).onReload = callback;

    LOG_INFO("Schedule reload for module with ID {}", id);
}

