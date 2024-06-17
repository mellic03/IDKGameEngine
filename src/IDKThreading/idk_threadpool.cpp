#include "idk_threadpool.hpp"
using namespace idk::ThreadPool;


#include <thread>
#include <atomic>
#include <mutex>

#include <vector>
#include <mutex>


namespace
{
    std::mutex           m_mutex;
    std::vector<fn_type> m_tasks;
    std::vector<fn_type> m_callbacks;
}


void
idk::ThreadPool::createTask( fn_type work, fn_type callback )
{
    // m_mutex.lock
    // m_tasks.push_back
}




