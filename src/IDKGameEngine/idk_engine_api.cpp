#include "idk_engine_api.hpp"

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKECS/IDKECS.hpp>
#include <IDKECS/IDKECS.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>

#include <IDKGraphics/IDKGraphics.hpp>
#include <libidk/idk_eventemitter.hpp>
#include <IDKIO/IDKIO.hpp>

#include <IDKThreading/IDKThreading.hpp>

static std::string gpath;

idk::EngineAPI::EngineAPI( idk::Game *game, int gl_major, int gl_minor )
:   m_game(game)
{
    m_engine   = new idk::Engine(this);
    m_events   = new idk::EventEmitter<std::string, void*>();
    m_io       = new idk::IO();
    m_audio    = new idk::Audio(memory.mainblock);
    m_tasks    = new idk::Threadpool(4, memory.mainblock);

    m_win      = new idk::Window(m_name.c_str(), 1920, 1080);
    m_gl       = new idk::GLContext(*m_win, gl_major, gl_minor);
    m_renderer = new idk::RenderEngine(*m_win, *m_gl, *m_tasks, memory.mainblock);

    createScene("DefaultScene");

    idk::RuntimeScript::threadpool = m_tasks;

    m_name = m_game->getName();
    m_game->setup(*this);

}


void
idk::EngineAPI::update( float dt )
{
    m_dt = dt;

    if (m_callbacks.size() > 0)
    {
        for (auto &callback: m_callbacks)
        {
            callback();
        }

        m_callbacks.clear();
    }
}


void
idk::EngineAPI::updateScenes()
{
    for (auto &[name, ecs]: m_scenes)
    {
        ecs->update(*this);
    }
}


bool
idk::EngineAPI::running()
{
    return getEngine().running();
}


void
idk::EngineAPI::shutdown()
{
    m_game->shutdown();
    getEngine().shutdown();
}


// void
// idk::EngineAPI::reloadEngine()
// {

// }


// void
// idk::EngineAPI::reloadECS( bool now )
// {
//     const auto callback = [this]() {
//         if (m_scenes.contains("DefaultScene"))
//         {
//             delete m_scenes["DefaultScene"];
//         }

//         createScene("DefaultScene");
//     };

//     if (now) callback();
//     else     m_callbacks.push_back(callback);
// }


// void
// idk::EngineAPI::reloadGL()
// {
//     m_callbacks.push_back([this]() {
//         m_gl->reload();
//     });
// }


// void
// idk::EngineAPI::reloadRenderer()
// {
//     m_callbacks.push_back([this]() {
//         if (m_renderer) delete m_renderer;
//         m_gl->reload();
//         m_renderer = new idk::RenderEngine(*m_win, *m_gl);
//     });
// }


// void
// idk::EngineAPI::reloadAll()
// {
//     // reloadEngine();
//     // reloadECS();
//     // reloadRenderer();
// }


idk::ECS*
idk::EngineAPI::createScene( const std::string &name )
{
    if (m_scenes.contains(name))
    {
        return nullptr;
    }

    m_scenes[name] = new idk::ECS(*this, name);
    auto &ecs = *(m_scenes[name]);

    idk::registerComponents(ecs);
    idk::registerSystems(ecs);

    return m_scenes[name];
}


idk::ECS&
idk::EngineAPI::getScene( const std::string &name )
{
    IDK_ASSERT("Scene does not exist!", m_scenes.contains(name));
    return *(m_scenes[name]);
}


bool
idk::EngineAPI::deleteScene( const std::string &name )
{
    if (m_scenes.contains(name) == false)
    {
        return false;
    }

    delete m_scenes[name];
    return true;
}
