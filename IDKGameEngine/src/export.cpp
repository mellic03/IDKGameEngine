#include <libidk/idk_export.hpp>

#include "idk_engine_api.hpp"
#include "IDKmodules/idk_module_export.hpp"


IDK_EXPORT_API
(
    idk::EngineAPI *idk_export_getEngineAPI( const char *name )
    {
        static idk::EngineAPI *api = nullptr;

        if (api == nullptr)
        {
            api = new idk::EngineAPI(name);
        }

        return api;
    }
)

