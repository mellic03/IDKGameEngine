#pragma once

#include <libidk/idk_export.hpp>
#include <string>


namespace idk
{
    class Engine;
    class RenderEngine;
    class ThreadPool;

    class EngineAPI;
};



class IDK_VISIBLE idk::EngineAPI
{
private:
    std::string         m_name;

public:
    idk::RenderEngine   *renderer_ptr   = nullptr;
    idk::Engine         *engine_ptr     = nullptr;
    idk::ThreadPool     *threadpool_ptr = nullptr;

    idk::Engine         &getEngine()     { return *engine_ptr;     };
    idk::RenderEngine   &getRenderer()   { return *renderer_ptr;   };
    idk::ThreadPool     &getThreadPool() { return *threadpool_ptr; };

    EngineAPI( const std::string &name ): m_name(name) {  };

};
