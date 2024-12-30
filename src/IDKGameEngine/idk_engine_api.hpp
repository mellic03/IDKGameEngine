#pragma once

#include <libidk/idk_export.hpp>
#include <libidk/idk_window.hpp>
#include <string>
#include <vector>
#include <functional>




namespace idk
{
    class Window;
    class GLContext;

    class EventSystem;
    // class AudioSystem;

    class Engine;
    class ECS;
    class IO;

    class RenderEngine;
    // class ThreadPool;
    class Packager;

    class Game;

    class EngineAPI;
}



class IDK_VISIBLE idk::EngineAPI
{
private:
    std::string         m_name;
    float               m_dtime = 0.001f;

    std::vector<std::string> m_args;
    std::vector<std::function<void()>> m_callbacks;


    idk::Window         *m_win      = nullptr;
    idk::GLContext      *m_gl       = nullptr;

    idk::Engine         *m_engine   = nullptr;
    idk::ECS            *m_ecs      = nullptr;
    idk::IO             *m_io       = nullptr;

    idk::RenderEngine   *m_renderer = nullptr;
    idk::Packager       *m_pkg      = nullptr;

    idk::Game           *m_game     = nullptr;


public:
                         EngineAPI( const std::vector<std::string>&, idk::Game*, int, int );

    void                 update( float dt );
    float                dtime() { return m_dtime; };

    idk::Window         &getWindow()     { return *m_win;      };
    idk::GLContext      &getGL()         { return *m_gl;       };
    idk::Engine         &getEngine()     { return *m_engine;   };
    idk::ECS            &getECS()        { return *m_ecs;      };
    idk::IO             &getIO()         { return *m_io;       };
    idk::RenderEngine   &getRenderer()   { return *m_renderer; };
    idk::Packager       &getPackager()   { return *m_pkg;      };
    idk::Game           *getGame()       { return  m_game;     };

    void                 reloadEngine();
    void                 reloadECS();
    void                 reloadGL();
    void                 reloadRenderer();
    void                 reloadGame();
    void                 reloadAll();

    bool                 running();
    void                 shutdown();

};
