#include "idk_engine_api.hpp"
#include "packager.hpp"

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKECS/IDKECS.hpp>
#include <IDKECS/ecs1/idk_ecs.hpp>
#include <IDKBuiltinCS/IDKBuiltinCS.hpp>

#include <IDKGraphics/IDKGraphics.hpp>
#include <IDKIO/IDKIO.hpp>
#include <libidk/idk_game.hpp>

#include <IDKThreading/IDKThreading.hpp>


idk::EngineAPI::EngineAPI( const std::vector<std::string> &args, idk::Game *game,
                           int gl_major, int gl_minor )
:   m_args(args)
{
    m_name = game->getName();

    m_engine   = new idk::Engine();
    m_ecs      = new idk::ECS();

    m_io       = new idk::IO();
    m_pkg      = new idk::Packager();

    m_win      = new idk::Window(m_name.c_str(), 1920, 1080);
    m_gl       = new idk::GLContext(*m_win, gl_major, gl_minor);
    m_renderer = new idk::RenderEngine(*m_win, *m_gl);

    m_game     = game;

    idk::registerComponents(*m_ecs);
    idk::registerSystems(*m_ecs);
};


void
idk::EngineAPI::update( float dt )
{
    m_dtime = dt;

    if (m_callbacks.size() > 0)
    {
        for (auto &callback: m_callbacks)
        {
            callback();
        }

        m_callbacks.clear();
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
    getEngine().shutdown();
}


void
idk::EngineAPI::reloadEngine()
{

}


void
idk::EngineAPI::reloadECS()
{
    m_callbacks.push_back([this]() {
        if (m_ecs) delete m_ecs;
        m_ecs = new idk::ECS();
        idk::registerComponents(*m_ecs);
        idk::registerSystems(*m_ecs);
    });
}


void
idk::EngineAPI::reloadGL()
{
    m_callbacks.push_back([this]() {
        m_gl->reload();
    });
}


void
idk::EngineAPI::reloadRenderer()
{
    m_callbacks.push_back([this]() {
        if (m_renderer) delete m_renderer;
        m_gl->reload();
        m_renderer = new idk::RenderEngine(*m_win, *m_gl);
    });
}

void
idk::EngineAPI::reloadGame()
{
    reloadECS();

    m_callbacks.push_back([this]() {
        if (m_game)
        {            
            m_game->setup(m_args, *this);
        }
    });
}


void
idk::EngineAPI::reloadAll()
{
    reloadEngine();
    reloadECS();
    reloadRenderer();
}

