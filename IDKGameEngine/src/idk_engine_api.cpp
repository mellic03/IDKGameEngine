#include "idk_engine_api.hpp"
#include <libidk/idk_export.hpp>


IDK_VISIBLE
idk::EngineAPI::EngineAPI( const std::string &name )
: m_renderer (name, 1920, 1080, 4, 6),
  m_engine   (m_renderer)
{

}

IDK_VISIBLE
idk::Engine&
idk::EngineAPI::getEngine()
{
    return m_engine;
}


IDK_VISIBLE
idk::RenderEngine&
idk::EngineAPI::getRenderer()
{
    return m_renderer;
}





#include <libidk/idk_platform.hpp>

#include <dlfcn.h>
// #include <SDL2/SDL_loadso.h>
#include <string>
#include <filesystem>


IDK_VISIBLE
idk::APILoader::APILoader( const char *relpath )
{
    #ifdef IDK_UNIX
        std::string extension = ".so";
    #elif defined(IDK_WINDOWS)
        std::string extension = ".a";
    #endif


    std::string path = std::filesystem::absolute(
        std::string(relpath) + extension
    );

    // m_lib     = SDL_LoadObject(path.c_str());
    // m_getAPI  = SDL_LoadFunction(m_lib, "idk_export_getEngineAPI");

    m_lib    = dlopen(path.c_str(), RTLD_LAZY);
    m_getAPI = dlsym(m_lib, "idk_export_getEngineAPI");

}


IDK_VISIBLE
idk::APILoader::~APILoader()
{
    SDL_UnloadObject(m_lib);
}


IDK_VISIBLE
idk::EngineAPI *
idk::APILoader::getEngineAPI()
{
    return (*(functionptr_type)(m_getAPI))();
}

