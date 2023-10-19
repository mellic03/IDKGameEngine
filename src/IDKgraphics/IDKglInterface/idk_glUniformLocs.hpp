#pragma once

#include "IDKcommon/IDKcommon.h"
#include "idk_glBindings.hpp"

namespace idk { class  glUniformLocs; };



class idk::glUniformLocs
{
private:
    struct DefaultLoc { GLint value = -1; };
    std::map<std::string, DefaultLoc> m_locations;

public:
    GLint get( const std::string &name );

};

