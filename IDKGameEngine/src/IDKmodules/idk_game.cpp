#include "idk_game.hpp"
#include <libidk/idk_platform.hpp>
#include <libidk/idk_export.hpp>

#include <iostream>
#include <filesystem>
#include <string>
#include <SDL2/SDL.h>


IDK_VISIBLE
idk::GameHandle::GameHandle( std::string relpath )
{
    #ifdef IDK_UNIX
        std::string extension = ".so";
    #elif defined(IDK_WINDOWS)
        std::string extension = ".a";
    #endif

    m_filepath = std::filesystem::absolute(relpath + extension);
    std::cout << "m_filepath: " << m_filepath << "\n";

    m_handle = SDL_LoadObject(m_filepath.c_str());

    if (m_handle == NULL)
    {
        std::cout << SDL_GetError() << "\n";
        exit(1);
    }

}


IDK_VISIBLE
idk::GameHandle::~GameHandle()
{
    SDL_UnloadObject(m_handle);
}


IDK_VISIBLE
idk::Game *
idk::GameHandle::getInstance()
{
    m_function = SDL_LoadFunction(m_handle, "getInstance");
    if (m_function == NULL)
    {
        std::cout << SDL_GetError() << "\n";
        exit(1);
    }

    return (*(getInstance_handle)(m_function))();
};

