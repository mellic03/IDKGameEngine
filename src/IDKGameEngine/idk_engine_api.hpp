#pragma once

#include <libidk/idk_export.hpp>
#include <libidk/idk_window.hpp>
#include <string>



namespace idk
{
    class Window;
    class GLContext;

    class EventSystem;
    // class AudioSystem;

    class Engine;
    class RenderEngine;
    // class ThreadPool;
    class Packager;

    class EngineAPI;
}



class IDK_VISIBLE idk::EngineAPI
{
private:
    std::string         m_name;
    float               m_dtime = 0.001f;

    idk::Window         *m_win      = nullptr;
    idk::GLContext      *m_gl       = nullptr;
    idk::Engine         *m_engine   = nullptr;
    idk::RenderEngine   *m_renderer = nullptr;
    idk::Packager       *m_pkg      = nullptr;

public:

    bool flag = false;


    void                 init( const std::string &, int, int );
    void                 update( float dt) { m_dtime = dt; };
    float                dtime() { return m_dtime; };

    idk::Window         &getWindow()     { return *m_win;      };
    idk::GLContext      &getGL()         { return *m_gl;       };
    idk::Engine         &getEngine()     { return *m_engine;   };
    idk::RenderEngine   &getRenderer()   { return *m_renderer; };
    idk::Packager       &getPackager()   { return *m_pkg;      };

    void                 reloadEngine();
    void                 reloadRenderer();
    void                 actuallyReloadRenderer();
    void                 reload();

    bool                 running();
    void                 shutdown();

};
