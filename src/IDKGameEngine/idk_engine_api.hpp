#pragma once

#include <libidk/idk_export.hpp>
#include <string>


namespace idk
{
    class EventSystem;
    // class AudioSystem;

    class Engine;
    class RenderEngine;
    // class ThreadPool;

    class EngineAPI;
};



class IDK_VISIBLE idk::EngineAPI
{
private:
    std::string         m_name;
    float               m_dtime = 0.001f;

public:
    idk::Engine         *engine_ptr     = nullptr;
    idk::RenderEngine   *renderer_ptr   = nullptr;

    void                 init( const std::string &, int, int );
    void                 update( float dt) { m_dtime = dt; };
    float                dtime() { return m_dtime; };

    idk::Engine         &getEngine()     { return *engine_ptr;     };
    idk::RenderEngine   &getRenderer()   { return *renderer_ptr;   };

};
