// #include <libidk/idk_export.hpp>
// #include "idk_engine_api.hpp"

// #include <IDKEvents/IDKEvents.hpp>
// #include <IDKAudio/IDKAudio.hpp>

// #include <IDKGameEngine/IDKGameEngine.hpp>
// #include <IDKGraphics/IDKGraphics.hpp>
// #include <IDKThreading/IDKThreading.hpp>

// #include <string>


// IDK_EXPORT_API
// (
//     idk::EngineAPI *getInstance()
//     {
//         static idk::EngineAPI *api = nullptr;

//         if (api == nullptr)
//         {
//             api = new idk::EngineAPI();
//         }

//         return api;
//     }
// )


// IDK_EXPORT_API
// (
//     void deleteInstance( void *instance )
//     {
//         idk::EngineAPI *api = reinterpret_cast<idk::EngineAPI *>(instance);
//         // api->deinit();
//         delete api;
//     }
// )
