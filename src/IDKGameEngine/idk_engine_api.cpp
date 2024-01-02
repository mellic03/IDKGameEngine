#include "idk_engine_api.hpp"


#include "idk_engine.hpp"
#include "IDKgraphics/render/idk_renderengine.hpp"
#include "IDKthreading/idk_threadpool.hpp"

#include <libidk/idk_platform.hpp>
#include "idk_dynamiclib.hpp"

#include <string>
#include <filesystem>


static void
instantiate( const char *name, idk::RenderEngine *&ren, idk::Engine *&engine, idk::ThreadPool *&threadpool )
{
    if (ren == nullptr)
    {
        ren        = new idk::RenderEngine(name, 1920, 1080, 4, 6);
        engine     = new idk::Engine(*ren);
        threadpool = new idk::ThreadPool();
    }
}


idk::Engine &
idk::EngineAPI::getEngine()
{
    instantiate(m_name, renderer_ptr, engine_ptr, threadpool_ptr);
    return *engine_ptr;
}


idk::RenderEngine &
idk::EngineAPI::getRenderer()
{
    instantiate(m_name, renderer_ptr, engine_ptr, threadpool_ptr);
    return *renderer_ptr;
}


idk::ThreadPool &
idk::EngineAPI::getThreadPool()
{
    instantiate(m_name, renderer_ptr, engine_ptr, threadpool_ptr);
    return *threadpool_ptr;
}




idk::APILoader::APILoader( const char *relpath )
{
    m_lib    = idk::dynamiclib::loadObject(relpath);
    m_getAPI = idk::dynamiclib::loadFunction(m_lib, "idk_export_getEngineAPI");
}


idk::APILoader::~APILoader()
{
    idk::dynamiclib::unloadObject(m_lib);
}


idk::EngineAPI &
idk::APILoader::getEngineAPI( const char *name )
{
    return *(*(functionptr_type)(m_getAPI))(name);
}


