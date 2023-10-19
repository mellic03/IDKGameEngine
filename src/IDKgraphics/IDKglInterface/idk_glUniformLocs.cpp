#include "idk_glUniformLocs.hpp"




GLint
idk::glUniformLocs::get( const std::string &name )
{
    GLint location = m_locations[name].value;

    #ifdef IDK_DEBUG
        if (location == -1)
        {
            std::cout << "[idk::glUniformLocs::get] location == -1\n";
            exit(1);
        }
    #endif

    return location;
}



