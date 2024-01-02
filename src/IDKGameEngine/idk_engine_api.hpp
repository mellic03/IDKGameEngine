#pragma once

#include <libidk/idk_export.hpp>


namespace idk
{
    class Engine;
    class RenderEngine;
    class ThreadPool;

    class EngineAPI;
    class APILoader;

    class Game;
    class GameLoader;
};



class IDK_VISIBLE idk::EngineAPI
{
private:
    const char        *m_name;

    idk::RenderEngine *renderer_ptr   = nullptr;
    idk::Engine       *engine_ptr     = nullptr;
    idk::ThreadPool   *threadpool_ptr = nullptr;

public:
    idk::Engine         &getEngine();
    idk::RenderEngine   &getRenderer();
    idk::ThreadPool     &getThreadPool();

    EngineAPI( const char *name ): m_name(name) {  };

};




class IDK_VISIBLE idk::APILoader
{
private:
    typedef idk::EngineAPI *(*functionptr_type)( const char * );

    void        *m_lib;
    void        *m_getAPI;

public:
        APILoader( const char *path );
       ~APILoader();

    idk::EngineAPI &getEngineAPI( const char * );
};



class IDK_VISIBLE idk::GameLoader
{
private:
    typedef idk::Game *(*functionptr_type)();

    void        *m_lib;
    void        *m_getGame;

public:
        GameLoader( const char *path );

    idk::Game *getGame( const char * );
};
