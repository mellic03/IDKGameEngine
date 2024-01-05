#include <libidk/idk_export.hpp>
#include "idk_engine_api.hpp"


IDK_EXPORT_API
(
    idk::EngineAPI *getInstance( const char *name )
    {
        static idk::EngineAPI *api = nullptr;

        if (api == nullptr)
        {
            api = new idk::EngineAPI(name);
        }

        return api;
    }
)

