#include "idk_module_handle.hpp"
#include <dlfcn.h>
#include <iostream>


idk::ModuleHandle::ModuleHandle( const char *filepath )
{
    m_lib = dlopen(filepath, RTLD_NOW | RTLD_GLOBAL);
    if (!m_lib)
    { 
        fprintf(stderr, "dlopen failure: %s\n", dlerror()); 
        exit(EXIT_FAILURE);
    }

    void *addr = dlsym(m_lib, "EditorUI2_getInstance");
    std::cout << "addr: " << addr << "\n";
    m_getInstance = reinterpret_cast<getInstance_handle>(addr);
}


idk::ModuleHandle::~ModuleHandle()
{
    dlclose(m_lib);
}


idk::RuntimeModule *
idk::ModuleHandle::getInstance()
{
    idk::RuntimeModule *instance = dynamic_cast<idk::RuntimeModule *>((*m_getInstance)());
    std::cout << "Instance address: " << instance << "\n";
    return instance;
}
