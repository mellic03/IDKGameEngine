#pragma once

#include "idk_runtime_module.hpp"


namespace idk { class ModuleHandle; };

class idk::ModuleHandle
{
private:
    typedef idk::RuntimeModule *(*getInstance_handle)();

    void *m_lib;
    void *m_addr_getInstance;
    getInstance_handle m_getInstance;

public:
        ModuleHandle( const char *filepath );
       ~ModuleHandle();

    idk::RuntimeModule *getInstance();
};

