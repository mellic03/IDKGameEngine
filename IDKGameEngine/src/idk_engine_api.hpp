#pragma once

#include "IDKengine.hpp"


namespace idk
{
    class EngineAPI;
    class APILoader;
};



class idk::EngineAPI
{
private:
    idk::RenderEngine   m_renderer;
    idk::Engine         m_engine;

public:
    EngineAPI( const std::string &name );

    idk::Engine         &getEngine();
    idk::RenderEngine   &getRenderer();

};


class idk::APILoader
{
private:
    typedef idk::EngineAPI *(*functionptr_type)();

    void        *m_lib;
    void        *m_getAPI;

public:
        APILoader( const char *path );
       ~APILoader();

    idk::EngineAPI *getEngineAPI();
};

