#pragma once

#include "IDKcommon/IDKcommon.h"
#include "idk_glBindings.hpp"

namespace idk { class glSSBO; };


class idk::glSSBO
{
private:
    GLuint m_SSBO;

public:

    void    init();
    void    bufferData( size_t nbytes, void *data );
    void    bufferSubData( size_t offset, size_t nbytes, void *data );
};


