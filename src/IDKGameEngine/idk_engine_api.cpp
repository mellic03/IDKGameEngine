#include "idk_engine_api.hpp"
#include "packager.hpp"

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKECS/IDKECS.hpp>

#include <IDKGraphics/IDKGraphics.hpp>
#include <IDKThreading/IDKThreading.hpp>



void
idk::EngineAPI::init( const std::string &name, int gl_major, int gl_minor )
{
    m_name = name;

    m_engine = new idk::Engine();
    m_pkg = new idk::Packager();

    m_win = new idk::Window(name.c_str(), 1920, 1080);
    m_gl = new idk::GLContext(*m_win, gl_major, gl_minor);
    m_renderer = new idk::RenderEngine(*m_win, *m_gl);
};



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
idk::EngineAPI::reloadRenderer()
{
    flag = true;
}


void
idk::EngineAPI::actuallyReloadRenderer()
{
    flag = false;

    if (m_renderer)
    {
        delete m_renderer;
    }

    m_gl->reload();
    m_renderer = new idk::RenderEngine(*m_win, *m_gl);
}


void
idk::EngineAPI::reload()
{
    reloadRenderer();
}
