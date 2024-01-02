#include "idk_dynamiclib.hpp"

#include <libidk/idk_assert.hpp>
#include <libidk/idk_platform.hpp>

#include <string>
#include <filesystem>


// #define IDK_DYNAMIC_LIBRARY_IDK
#define IDK_DYNAMIC_LIBRARY_SDL2


#ifdef IDK_UNIX
    #define IDK_DYNAMIC_LIBRARY_FILE_EXTENSION ".so"

#elif defined(IDK_WINDOWS)
    #define IDK_DYNAMIC_LIBRARY_FILE_EXTENSION ".dll"

#endif


#ifdef IDK_DYNAMIC_LIBRARY_IDK
    #include <dlfcn.h>
    #define IDK_DYNAMIC_LIBRARY_LOAD_OBJECT(filepath) dlopen(filepath, RTLD_LAZY)
    #define IDK_DYNAMIC_LIBRARY_LOAD_FUNCTION(lib, symbol) dlsym(lib, symbol)
    #define IDK_DYNAMIC_LIBRARY_UNLOAD_OBJECT(lib) dlclose(lib)

#elif defined(IDK_DYNAMIC_LIBRARY_SDL2)
    #include <SDL2/SDL_loadso.h>
    #define IDK_DYNAMIC_LIBRARY_LOAD_OBJECT(filepath) SDL_LoadObject(filepath)
    #define IDK_DYNAMIC_LIBRARY_LOAD_FUNCTION(lib, symbol) SDL_LoadFunction(lib, symbol)
    #define IDK_DYNAMIC_LIBRARY_UNLOAD_OBJECT(lib) SDL_UnloadObject(lib)

#endif




void *
idk::dynamiclib::loadObject( const char *filepath )
{
    std::string relpath   = filepath;
    std::string extension = IDK_DYNAMIC_LIBRARY_FILE_EXTENSION;
    std::string path = std::filesystem::absolute(relpath + extension);

    void *lib = IDK_DYNAMIC_LIBRARY_LOAD_OBJECT(path.c_str());

    std::string msg = "Could not load shared library from path: " + path;
    IDK_ASSERT(msg.c_str(), lib != nullptr);

    return lib;
}


void *
idk::dynamiclib::loadFunction( void *lib, const char *symbol )
{
    void *function_ptr = IDK_DYNAMIC_LIBRARY_LOAD_FUNCTION(lib, symbol);

    std::string msg = "Could not load symbol: " + std::string(symbol);
    IDK_ASSERT(msg.c_str(), function_ptr != nullptr);

    return function_ptr;
}


void
idk::dynamiclib::unloadObject( void *lib )
{
    IDK_DYNAMIC_LIBRARY_UNLOAD_OBJECT(lib);
}

