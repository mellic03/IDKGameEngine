#pragma once

#include "idk_allocator.h"

#include <functional>
#include <queue>
#include <set>

#include <thread>
#include <mutex>
#include <atomic>


namespace idk { class ThreadPool; };


class idk::ThreadPool
{
    using fun_t = std::function<void()>;

private:
    std::vector<std::thread>    m_threads;
    std::mutex                  m_mutex;
    idk::Allocator<fun_t>       m_tasks;
    std::queue<int>             m_task_ids;
    std::set<int>               m_finished_ids;
    std::atomic_bool            m_running;


public:
                        const static int nullid = -1;
                        ThreadPool( int num_threads );


    template <typename function, typename ...Args>
    int                 push    ( function fn, Args&&... args );

    int                 pop     (             );
    fun_t               get     ( int task_id );
    void                done    ( int task_id );
    void                join    (             );

    bool                running();
    void                stop();
};



template <typename function, typename ...Args>
int
idk::ThreadPool::push( function fn, Args&&... args )
{
    std::unique_lock<std::mutex> lock(m_mutex);

    int task_id = m_tasks.create(std::bind(fn, args...));
    m_task_ids.push(task_id);

    return task_id;
};


