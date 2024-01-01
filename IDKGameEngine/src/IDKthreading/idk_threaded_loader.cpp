#include "idk_threaded_loader.hpp"



void
idk::ThreadedLoader::_load_model( std::string filepath, int id )
{
    std::ifstream stream(filepath);
    std::cout << "[worker " + std::to_string(id) + "] " + filepath + "\n";
    stream.close();
};


void
idk::ThreadedLoader::_finish_loading( int id )
{
    std::cout << "[ThreadedLoader::_finish_loading] Finished loading model " + std::to_string(id) + "\n";
    m_num_loading--;
};


void
idk::ThreadedLoader::_destroy( int id )
{
    if (m_num_loading.load() == 0)
    {
        m_allocator.destroy(id);
    }

    else
    {
        // Push id into queue for deletion once no resources are currently loading.
    }
};

#include <memory>

int
idk::ThreadedLoader::loadModel( std::string filepath )
{
    int model_id = m_allocator.create();
    m_num_loading++;

    idk::ThreadPool &threadpool = idk::ThreadPool::get();
    std::string *str = new std::string(filepath.c_str());

    idk::ThreadPool::Task task(
        [str]()
        {
            std::cout << *str + "WOOP\n";
        },

        [str]()
        {
            std::cout << *str + "YAY!\n";
        }
    );

    int id = threadpool.createTask(task);
    threadpool.detach(id);

    std::cout << "[ThreadedLoader::loadModel] Sent model " << model_id << " to thread pool\n";

    return model_id;
};
