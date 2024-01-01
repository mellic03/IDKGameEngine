#include <libidk/idk_export.hpp>

#include "idk_engine_api.hpp"
#include "IDKmodules/idk_module_export.hpp"


IDK_EXPORT_API
(
    idk::EngineAPI *idk_export_getEngineAPI()
    {
        static bool first = true;

        if (first)
        {
            first = false;
            return new idk::EngineAPI("Test name");
        }

        return nullptr;
    }
)



