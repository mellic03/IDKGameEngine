#pragma once

#include <libidk/idk_export.hpp>
#include <string>


namespace idk
{
    class EventSystem;
    class AudioSystem;

    class Engine;
    class RenderEngine;
    class ThreadPool;

    class EngineAPI;
};


namespace idk::ecs { class ECS; };



class IDK_VISIBLE idk::EngineAPI
{
private:
    std::string         m_name;

public:
    idk::EventSystem    *eventsys_ptr   = nullptr;
    idk::AudioSystem    *audiosys_ptr   = nullptr;
    idk::ecs::ECS          *ecs_ptr        = nullptr;
    idk::Engine         *engine_ptr     = nullptr;
    idk::RenderEngine   *renderer_ptr   = nullptr;
    idk::ThreadPool     *threadpool_ptr = nullptr;

    void                 init( const std::string &, int, int );

    idk::EventSystem    &getEventSys()   { return *eventsys_ptr;   };
    idk::AudioSystem    &getAudioSys()   { return *audiosys_ptr;   };

    idk::ecs::ECS       &getECS()        { return *ecs_ptr;        };
    idk::Engine         &getEngine()     { return *engine_ptr;     };
    idk::RenderEngine   &getRenderer()   { return *renderer_ptr;   };
    idk::ThreadPool     &getThreadPool() { return *threadpool_ptr; };

};
