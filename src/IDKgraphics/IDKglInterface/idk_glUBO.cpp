#include "idk_glUBO.hpp"


idk::glUBO::glUBO( GLuint location, GLsizeiptr size )
{
    m_offset = 0;

    gl::genBuffers(1, &m_UBO);
    gl::bindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    gl::bufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    gl::bindBuffer(GL_UNIFORM_BUFFER, 0);
    gl::bindBufferBase(GL_UNIFORM_BUFFER, location, m_UBO);
}


void
idk::glUBO::bind()
{
    m_offset = 0;
    gl::bindBuffer(GL_UNIFORM_BUFFER, m_UBO);
};


void
idk::glUBO::unbind()
{
    m_offset = 0;
    gl::bindBuffer(GL_UNIFORM_BUFFER, 0);
};


void
idk::glUBO::add(GLintptr size, const void *data)
{
    gl::bufferSubData(GL_UNIFORM_BUFFER, m_offset, size, data);
    m_offset += idk::roundup(size, 16);
}

