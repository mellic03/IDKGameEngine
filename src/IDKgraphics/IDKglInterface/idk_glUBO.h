#pragma once

#include "IDKcommon/IDKcommon.h"
#include "idk_glBindings.h"

namespace idk { class glUBO; };


class idk::glUBO
{
private:
    GLuint      _UBO    = 0;
    GLintptr    _offset = 0;

public:
                glUBO() {  };
                glUBO( GLuint location, GLsizeiptr size );
    void        bind();
    void        unbind();

    template <typename data_t>
    void        add(const void *data);
    void        add(GLintptr size, const void *data);
};



template <typename T>
void
idk::glUBO::add(const void *data)
{
    gl::bufferSubData(GL_UNIFORM_BUFFER, _offset, sizeof(T), data);
    _offset += idk::roundup(sizeof(T), 16);
}

