#pragma once

#include "idk_threadpool.hpp"


namespace idk::internal { class ThreadPoolAPI; };

class idk::internal::ThreadPoolAPI
{
public:
    static void update( idk::ThreadPool &threadpool ) { threadpool._update(); };
};

