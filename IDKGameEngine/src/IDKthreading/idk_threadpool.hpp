#pragma once

#include <libidk/idk_singleton.hpp>
#include <libidk/idk_allocator.hpp>
#include <libidk/idk_export.hpp>

#include <functional>
#include <vector>
#include <queue>
#include <map>

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>


namespace idk { class ThreadPool; };
namespace idk::internal { class ThreadPoolAPI; };


class idk::ThreadPool: public idk::basic_singleton<idk::ThreadPool>
{
public:
    class Task;

    template <typename T>
    class ResourceWrapper;

    static constexpr int NUM_THREADS = 4;


private:
    friend class idk::basic_singleton<idk::ThreadPool>;
    friend class idk::internal::ThreadPoolAPI;

    using cond_var = std::condition_variable;

    int                             m_current_idx = 0;

    std::mutex                      m_main_mutex;
    std::condition_variable         m_main_cv;

    Allocator<Task>                 m_tasks;

    std::vector<std::mutex>         m_mutexes;
    std::vector<std::mutex>         m_finished_mutexes;
    std::vector<cond_var>           m_worker_cvs;
    std::vector<std::thread>        m_threads;

    std::vector<std::atomic_int>    m_tasks_waiting;
    std::vector<std::queue<int>>    m_task_queues;
    std::vector<std::queue<int>>    m_finished_tasks;

            ThreadPool();

    void    _update();

    void    _worker   ( int idx );
    Task    _pop_task ( int idx );


public:

    IDK_VISIBLE
    int createTask( const Task &task );

    IDK_VISIBLE
    int createTask( std::function<void()> work, std::function<void()> callback );

    IDK_VISIBLE
    void detach( int idx );

};



// template <typename ...Args>
// void
// idk::ThreadPool::detach( int id, Args ...rest )
// {
//     _detach(id);
//     detach(rest...);
// }




class idk::ThreadPool::Task
{
private:
    friend class idk::ThreadPool;
    using fn_type = std::function<void()>;

    // std::atomic_bool m_done;
    std::atomic_int  m_id;

    void    _setID( int id ) { m_id.store(id);     };
    int     _getID(        ) { return m_id.load(); };

    fn_type _work;
    fn_type _callback;


public:

    /**
     * @param work_fn function to execute on worker thread.
     * @param callback_fn function to execute on main thread after [work_fn] finishes.
    */
    Task( fn_type work, fn_type callback )
    : _work(work), _callback(callback)
    {
        // m_done.store(false);
        m_id.store(-1);
    };

    Task( const Task &rhs )
    : _work(rhs._work), _callback(rhs._callback)
    {
        // m_done.store(rhs.m_done.load());
        m_id.store(rhs.m_id.load());
    };

    Task( const Task &&rhs )
    : _work(rhs._work), _callback(rhs._callback)
    {
        // m_done.store(rhs.m_done.load());
        m_id.store(rhs.m_id.load());
    };

    Task &operator = ( const Task &rhs )
    {
        _work     = rhs._work;
        _callback = rhs._callback;
        // m_done.store(rhs.m_done.load());
        m_id.store(rhs.m_id.load());
        return *this;
    };

    void work()
    {
        _work();
        // m_done.store(true);
    };

    void callback()
    {
        _callback();
    };

    // bool is_done() const { return m_done.load(); };
};



template <typename T>
class idk::ThreadPool::ResourceWrapper
{
private:
    std::atomic_bool m_loaded;
    const T &m_ref;


public:
    ResourceWrapper( const T &ref )
    : m_loaded(false), m_ref(ref) {  };

    bool is_loaded() const { return m_loaded.load(); };
};
