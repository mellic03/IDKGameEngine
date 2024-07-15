#include "idk_threadpool.hpp"
using namespace idk::ThreadPool;

#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>

#include <vector>
#include <deque>
#include <mutex>


struct Task
{
    fn_type work     = [](){};
    fn_type callback = [](){};
    bool    is_good  = false;
};


static size_t NUM_THREADS = 2;

namespace
{
    std::mutex         m_mutex_running[16];
    std::mutex         m_mutex_complete[16];
    std::deque<Task>   m_tasks[16];
    std::deque<Task>   m_completed[16];
    std::thread        m_threads[16];
}




void
work_fn( int idx )
{
    while (true)
    {
        Task task;

        // Aquire work
        // ----------------------------------------------
        {
            std::lock_guard<std::mutex> lock(m_mutex_running[idx]);
            if (m_tasks[idx].size() > 0)
            {
                task = m_tasks[idx].front();
                       m_tasks[idx].pop_front();
            }
        }
        // ----------------------------------------------


        // Do work
        // ----------------------------------------------
        if (task.is_good == false)
        {
            continue;
        }

        task.work();
        // ----------------------------------------------



        // Push task ID to finished queue
        // ----------------------------------------------
        {
            std::lock_guard<std::mutex> lock(m_mutex_complete[idx]);
            m_completed[idx].push_back(task);
        }
        // ----------------------------------------------
    }
}



void
idk::ThreadPool::createTask( fn_type work, fn_type callback )
{
    static int idx = 0;

    {
        std::lock_guard<std::mutex> lock(m_mutex_running[idx]);    
        m_tasks[idx].push_back({work, callback, true});
    
        idx = (idx + 1) % NUM_THREADS;
    }

}


void
idk::ThreadPool::init( size_t threads )
{
    NUM_THREADS = threads;

    for (int i=0; i<NUM_THREADS; i++)
    {
        m_threads[i] = std::thread(work_fn, i);
        m_threads[i].detach();
    }
}


void
idk::ThreadPool::update()
{
    for (int i=0; i<NUM_THREADS; i++)
    {
        std::lock_guard<std::mutex> lock(m_mutex_complete[i]);

        while (m_completed[i].size() > 0)
        {
            m_completed[i].front().callback();
            m_completed[i].pop_front();
        }
    }
}



