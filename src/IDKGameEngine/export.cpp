#include <libidk/idk_export.hpp>
#include "idk_engine_api.hpp"

#include <IDKEvents/IDKEvents.hpp>
#include <IDKAudio/IDKAudio.hpp>

#include <IDKGameEngine/IDKGameEngine.hpp>
#include <IDKGraphics/IDKGraphics.hpp>
#include <IDKThreading/IDKThreading.hpp>

#include <string>


IDK_EXPORT_API
(
    idk::EngineAPI *getInstance( const std::string &name )
    {
        static idk::EngineAPI *api = nullptr;

        if (api == nullptr)
        {
            api = new idk::EngineAPI(name);

            api->eventsys_ptr   = new idk::EventSystem();
            api->audiosys_ptr   = new idk::AudioSystem();

            api->engine_ptr     = new idk::Engine();
            api->renderer_ptr   = new idk::RenderEngine(name, 1920, 1080, 4, 6);
            api->threadpool_ptr = new idk::ThreadPool();

        }

        return api;
    }
)

