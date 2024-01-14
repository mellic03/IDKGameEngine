#include <libidk/idk_export.hpp>
#include "idk_engine_api.hpp"

#include <IDKGameEngine/IDKengine.hpp>
#include <IDKGameEngine/IDKthreading/IDKthreading.hpp>
#include <IDKGraphics/IDKGraphics.hpp>

#include <string>


IDK_EXPORT_API
(
    idk::EngineAPI *getInstance( const std::string &name )
    {
        static idk::EngineAPI *api = nullptr;

        if (api == nullptr)
        {
            api = new idk::EngineAPI(name);
            api->renderer_ptr   = new idk::RenderEngine(name, 1920, 1080, 4, 6);
            api->engine_ptr     = new idk::Engine(*api->renderer_ptr);
            api->threadpool_ptr = new idk::ThreadPool();
        }

        return api;
    }
)

