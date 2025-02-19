#include "idk_engine.hpp"

#include <libidk/idk_export.hpp>
#include <libidk/idk_platform.hpp>
#include <libidk/idk_scene_file.hpp>
#include <libidk/idk_log2.hpp>

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
idk::Engine::_reloadModules()
{
    for (auto &loader: m_modules)
    {
        loader.reload();
        loader.getInstance()->init(*m_api);
    }
}


void
idk::Engine::_clearModules()
{
    m_modules.clear();
}



void
idk::Engine::_idk_modules_stage_A( idk::EngineAPI &api )
{
    for (auto &loader: m_modules)
    {
        loader.getInstance()->stage_A(api);
    }
}


void
idk::Engine::_idk_modules_stage_B( idk::EngineAPI &api )
{
    for (auto &loader: m_modules)
    {
        loader.getInstance()->stage_B(api);
    }
}


void
idk::Engine::_idk_modules_stage_C( idk::EngineAPI &api )
{
    for (auto &loader: m_modules)
    {
        loader.getInstance()->stage_C(api);
    }
}



IDK_VISIBLE
void
idk::Engine::beginFrame( idk::EngineAPI &api, float dt )
{
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

    if (m_reload)
    {
        _reloadModules();
        m_reload = false;
    }

    if (m_clear || m_running == false)
    {
        _clearModules();
        m_clear = false;
    }
}


void
idk::Engine::shutdown()
{
    LOG_INFO("idk::Engine::shutdown");
    m_running = false;
}



int
idk::Engine::registerModule( const std::string &filename )
{
    using ModuleLoader = idk::GenericLoader<idk::Module>;
    namespace fs = std::filesystem;

    m_modules.push_back(ModuleLoader(filename + IDK_DLIB_EXT));
    m_modules.back().getInstance()->init(*m_api);

    std::cout << "[Engine::registerModule] Loaded module \"" << filename << "\"\n";

    return 0;
}
