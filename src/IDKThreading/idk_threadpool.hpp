#pragma once

#include <functional>



namespace idk::ThreadPool
{
    using fn_type = std::function<void()>;

    void createTask( fn_type work, fn_type callback );

};





