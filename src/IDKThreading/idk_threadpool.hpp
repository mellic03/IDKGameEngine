#pragma once

#include <cstddef>
#include <cstdint>

#include <functional>
#include <atomic>


namespace idk::ThreadPool
{
    using fn_type = std::function<void()>;

    void createTask( fn_type work, fn_type callback );
    void init( size_t threads );
    void update();

};





