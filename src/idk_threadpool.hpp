// #pragma once

// #include "libidk/libidk.hpp"

// #include <functional>
// #include <queue>
// #include <set>

// #include <thread>
// #include <mutex>
// #include <atomic>


// namespace idk { class ThreadPool; };


// class idk::ThreadPool
// {
//     using fun_t = std::function<void()>;

// private:
//     std::vector<std::thread>    m_threads;
//     std::mutex                  m_mutex;
//     idk::Allocator<fun_t>       m_tasks;
//     std::queue<int>             m_task_queue;
//     std::set<int>               m_finished_ids;
//     std::atomic_bool            m_running;


// public:
//                         const static int nullid = -1;
//                         ThreadPool( int num_threads );

//     int                 pop     (             );
//     fun_t               get     ( int task_id );
//     void                done    ( int task_id );

//     template <typename function_t, typename ...Args>
//     int                 push  ( function_t fn, Args&&... args );
//     bool                isDone  ( int task_id );
//     void                destroy ( int task_id );

//     // void                join( );
//     bool                running();
//     void                stop();
// };



// template <typename function_t, typename ...Args>
// int
// idk::ThreadPool::push( function_t fn, Args&&... args )
// {
//     std::unique_lock<std::mutex> lock(m_mutex);

//     int task_id = m_tasks.create(std::bind(fn, args...));
//     m_task_queue.push(task_id);

//     return task_id;
// };


