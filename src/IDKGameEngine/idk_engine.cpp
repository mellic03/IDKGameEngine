#include "idk_engine.hpp"

#include <libidk/idk_export.hpp>
#include <libidk/idk_platform.hpp>
#include <libidk/idk_scene_file.hpp>
#include <libidk/idk_log.hpp>

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

    for (auto &loader: m_modules)
    {
        loader.getInstance()->init(api);
    }
}



void
idk::Engine::_reloadModules()
{
    for (auto &loader: m_modules)
    {
        loader.reload();
        loader.getInstance()->init(*api_ptr);
    }
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

    if (m_running == false)
    {
        m_modules.clear();
    }
}


void
idk::Engine::shutdown()
{
    LOG_INFO() << "Shutdown initiated";
    m_running = false;
}



int
idk::Engine::registerModule( const std::string &filename )
{
    using ModuleLoader = idk::GenericLoader<idk::Module>;
    namespace fs = std::filesystem;

    #ifdef IDK_UNIX
        std::string path = filename + ".so";
    #elif defined(IDK_WINDOWS)
        std::string path = filename + ".dll";
    #endif

    m_modules.push_back(ModuleLoader(path));
    LOG_INFO() << "Loaded module \"" << path << "\"";

    return 0;
}
