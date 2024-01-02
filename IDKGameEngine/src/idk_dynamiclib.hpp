#pragma once

#include <libidk/idk_export.hpp>


namespace idk::dynamiclib
{
    IDK_VISIBLE void *loadObject( const char *filepath );
    IDK_VISIBLE void *loadFunction( void *lib, const char *symbol );
    IDK_VISIBLE void  unloadObject( void *lib );
};
