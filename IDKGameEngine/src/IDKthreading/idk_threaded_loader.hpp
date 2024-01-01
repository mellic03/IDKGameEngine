#pragma once

#include "idk_threadpool.hpp"
#include "idk_threaded_allocator.hpp"
#include "../IDKgraphics/animation/idk_animation_file.hpp"

#include <string>
#include <functional>



namespace idk
{
    class ThreadedResource;
    class ThreadedLoader;
};



class idk::ThreadedResource
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



struct idk_Model
{
    float    *vertices;
    uint32_t *indices;
};



class idk::ThreadedLoader
{
private:

    idk::Allocator<idk_Model>   m_allocator;
    std::atomic_int             m_num_loading;


public:

    void _load_model( std::string filepath, int id );
    void _finish_loading( int id );
    void _destroy( int id );

    ThreadedLoader()
    {
        m_num_loading.store(0);
    
        m_allocator.create();
        m_allocator.create();
        m_allocator.create();
        m_allocator.create();

        m_allocator.destroy(3);
        m_allocator.destroy(2);
        m_allocator.destroy(1);
        m_allocator.destroy(0);

    };

    int loadModel( std::string filepath );

};


