#include "idk_engine_api.hpp"
#include "packager.hpp"

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKECS/IDKECS.hpp>

#include <IDKGraphics/IDKGraphics.hpp>
#include <IDKThreading/IDKThreading.hpp>




void
idk::EngineAPI::init( const std::string &windowname, int gl_major, int gl_minor )
{
    m_name = windowname;

    engine_ptr     = new idk::Engine();
    renderer_ptr   = new idk::RenderEngine(m_name, 1920, 1080, gl_major, gl_minor);
    pkg_ptr        = new idk::Packager();
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


