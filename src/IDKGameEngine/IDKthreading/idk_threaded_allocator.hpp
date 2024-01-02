#pragma once

#include <libidk/idk_allocator.hpp>

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>


namespace idk { template <typename T> class ThreadedAllocator; };


template <typename T>
class idk::ThreadedAllocator
{
private:
    idk::Allocator<T>       m_allocator;
    std::vector<std::mutex> m_mutexes;
    std::mutex              m_mutex;

public:

    // int create()
    // {
    //     std::unique_lock lock(m_mutex);
    //     int id = m_allocator.create();

    //     if (id > m_mutexes.size())
    //     {
    //         m_mutexes.resize(id+1);
    //     }

    //     return 
    // };

    // void destroy( int id )
    // {
    //     std::unique_lock lock(m_mutex);

    //     m_allocator.destroy(id);
    // };


    // T &get( int id )
    // {
    //     std::unique_lock lock1(m_mutex);
    //     std::unique_lock lock2(m_mutexes[id]);
    //     return m_allocator.get(id);
    // };


    // std::mutex &mutex()
    // {
    //     return m_mutex
    // };

};


