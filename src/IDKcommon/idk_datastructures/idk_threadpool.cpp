#include "idk_threadpool.hpp"
#include <iostream>


static void
idk_threadpool_worker( idk::ThreadPool &wq )
{
    while (1)
    {
        while (1)
        {
            int task_id = wq.pop();

            if (task_id != idk::ThreadPool::nullid)
            {
                auto function = wq.get(task_id);
                function();

                wq.done(task_id);
            }
        }


        if (wq.running() == false)
        {
            break;
        }
    }
}



idk::ThreadPool::ThreadPool( int num_threads )
{
    m_running.store(true);
    m_threads = std::vector<std::thread>(num_threads);

    for (int i=0; i<num_threads; i++)
    {
        m_threads[i] = std::thread(idk_threadpool_worker, std::ref(*this));
        m_threads[i].detach();
    }
}



int 
idk::ThreadPool::pop()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_task_ids.empty())
    {
        return nullid;
    }

    int task_id = m_task_ids.front();  m_task_ids.pop();

    return task_id;
};



std::function<void()>
idk::ThreadPool::get( int task_id )
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_tasks.get(task_id);;
};



void
idk::ThreadPool::done( int task_id )
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_finished_ids.emplace(task_id);
    m_tasks.remove(task_id);
};



void
idk::ThreadPool::join()
{
    while (1)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (m_tasks.size() == 0)
        {
            m_finished_ids.erase(m_finished_ids.begin(), m_finished_ids.end());
            break;
        }

        lock.unlock();
    }

}
