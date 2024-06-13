#include "idk_threadpool.hpp"



idk::ThreadPool::ThreadPool()
: m_tasks_waiting    (NUM_THREADS),
  m_task_queues      (NUM_THREADS),
  m_finished_tasks   (NUM_THREADS),
  m_mutexes          (NUM_THREADS),
  m_finished_mutexes (NUM_THREADS),
  m_worker_cvs       (NUM_THREADS)
{
    for (int i=0; i<NUM_THREADS; i++)
    {
        m_threads.push_back(std::thread(&ThreadPool::_worker, std::ref(*this), i));
        m_tasks_waiting[i].store(0);
    }
}


void
idk::ThreadPool::_update()
{
    for (int i=0; i<NUM_THREADS; i++)
    {
        // std::unique_lock lock(m_finished_mutexes[i]);
        std::unique_lock lock(m_main_mutex);
        auto &task_ids = m_finished_tasks[i];

        while (task_ids.size() > 0)
        {
            int task_id = task_ids.front();
                          task_ids.pop();

            Task task = m_tasks.get(task_id);
            task.callback();
            m_tasks.destroy(task_id);
        }

        lock.unlock();
    }
}



void
idk::ThreadPool::_worker( int idx )
{
    while (true)
    {
        // Aquire work
        // ----------------------------------------------
        std::unique_lock lock(m_main_mutex);

        m_worker_cvs[idx].wait(lock, [this, idx] {
            return m_tasks_waiting[idx].load() > 0;
        });

        Task task  = _pop_task(idx);
        int task_id = task._getID();
        // ----------------------------------------------


        // Do work
        // ----------------------------------------------
        task.work();
        // ----------------------------------------------



        // Push task ID to finished queue
        // ----------------------------------------------
    
        std::cout << "Finished task " + std::to_string(task_id) + "\n";
        m_finished_tasks[idx].push(task_id);

        lock.unlock();
        // ----------------------------------------------
    }
}



idk::ThreadPool::Task
idk::ThreadPool::_pop_task( int idx )
{
    int task_id = m_task_queues[idx].front();
                  m_task_queues[idx].pop();

    m_tasks_waiting[idx]--;

    return m_tasks.get(task_id);
}


int
idk::ThreadPool::createTask( const Task &task )
{
    int task_id = m_tasks.create(task);
    m_tasks.get(task_id)._setID(task_id);
    return task_id;
}


int
idk::ThreadPool::createTask( std::function<void()> work, std::function<void()> callback )
{
    int task_id = m_tasks.create(Task(work, callback));
    m_tasks.get(task_id)._setID(task_id);
    return task_id;
}


void
idk::ThreadPool::detach( int task_id )
{
    int idx = m_current_idx;
    m_current_idx = (idx + 1) % NUM_THREADS;

    // std::unique_lock lock(m_mutexes[idx]);
    std::unique_lock lock(m_main_mutex);
    m_task_queues[idx].push(task_id);
    m_tasks_waiting[idx]++;
    lock.unlock();

    m_worker_cvs[idx].notify_all();

}

