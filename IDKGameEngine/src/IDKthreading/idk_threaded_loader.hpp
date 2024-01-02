#pragma once

#include "idk_threadpool.hpp"
#include "idk_threaded_allocator.hpp"
#include "../IDKgraphics/animation/idk_animation_file.hpp"

#include <libidk/idk_export.hpp>

#include <string>
#include <functional>



namespace idk
{
    class ThreadedResource;
    class ThreadedLoader;
};



class IDK_VISIBLE idk::ThreadedResource
{
private:
    std::atomic_bool m_loaded;

public:

    ThreadedResource()
    {
        m_loaded.store(false);
    };

    ThreadedResource( const ThreadedResource &rhs )
    {
        m_loaded.store(rhs.m_loaded.load());
    };

    ThreadedResource( const ThreadedResource &&rhs )
    {
        m_loaded.store(rhs.m_loaded.load());
    };

    bool is_loaded() { return m_loaded.load(); };

};



struct IDK_VISIBLE idk_Model
{
    float    *vertices;
    uint32_t *indices;
};



class IDK_VISIBLE idk::ThreadedLoader
{
private:
    idk::ThreadPool            &m_threadpool;

    idk::Allocator<idk_Model>   m_allocator;
    std::atomic_int             m_num_loading;


public:
        ThreadedLoader( idk::ThreadPool &threadpool );

    void _load_model( std::string filepath, int id );
    void _finish_loading( int id );
    void _destroy( int id );

    int loadModel( std::string filepath );

};


