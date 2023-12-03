#pragma once

#include "idk_glBindings.hpp"

namespace idk { class glSSBO; };


class idk::glSSBO
{
private:
    GLuint m_SSBO;
    GLuint m_index;

public:

    void    init( GLuint index );
    void    bufferData( size_t nbytes, void *data );
    void    bufferSubData( size_t offset, size_t nbytes, void *data );

    void *  mapBuffer();
    void    unmapBuffer();

};


