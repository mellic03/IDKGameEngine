#pragma once

#include "idk_glBindings.hpp"

namespace idk { class glUBO; };

int round_up( int n, int multipleof );

class idk::glUBO
{
private:
    GLuint      m_UBO;
    GLintptr    m_offset;

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
    gl::bufferSubData(GL_UNIFORM_BUFFER, m_offset, sizeof(T), data);
    m_offset += round_up(sizeof(T), 16);
}

