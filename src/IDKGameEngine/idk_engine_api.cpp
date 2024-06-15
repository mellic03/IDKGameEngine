#include "idk_engine_api.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKAudio/IDKAudio.hpp>

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKECS/IDKECS.hpp>

#include <IDKGraphics/IDKGraphics.hpp>
#include <IDKThreading/IDKThreading.hpp>




void
idk::EngineAPI::init( const std::string &windowname, int gl_major, int gl_minor )
{
    m_name = windowname;

    eventsys_ptr   = new idk::EventSystem();
    audiosys_ptr   = new idk::AudioSystem();
    // ecs_ptr        = new idk::ecs::ECS(*this);
    engine_ptr     = new idk::Engine();
    renderer_ptr   = new idk::RenderEngine(m_name, 1920, 1080, gl_major, gl_minor);
    threadpool_ptr = new idk::ThreadPool();
};


