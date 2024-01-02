#pragma once

#include "idk_dynamiclib.hpp"
#include <libidk/idk_export.hpp>

namespace idk { class GenericAPILoader; };


class IDK_VISIBLE idk::GenericAPILoader
{
private:
    void *m_lib;


public:
    GenericAPILoader( const char *path )
    {
        m_lib = idk::dynamiclib::loadObject(path);
    };

    ~GenericAPILoader()
    {
        idk::dynamiclib::unloadObject(m_lib);
    };

    template <typename return_type, typename ...Args>
    return_type *getAPI( const char *symbol, Args ...args );

};


template <typename return_type,typename ...Args>
return_type *
idk::GenericAPILoader::getAPI( const char *symbol, Args ...args )
{
    typedef return_type *(*functionptr_type)( Args... );
    void *ptr = idk::dynamiclib::loadFunction(m_lib, symbol);

    return (*(functionptr_type)(ptr))(args...);
};

