#include "idk_glUBO.h"


idk::glUBO::glUBO( GLuint location, GLsizeiptr size )
{
    _offset = 0;

    gl::genBuffers(1, &_UBO);
    gl::bindBuffer(GL_UNIFORM_BUFFER, _UBO);
    gl::bufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    gl::bindBuffer(GL_UNIFORM_BUFFER, 0);
    gl::bindBufferBase(GL_UNIFORM_BUFFER, location, _UBO);
}


void
idk::glUBO::bind()
{
    _offset = 0;
    gl::bindBuffer(GL_UNIFORM_BUFFER, _UBO);
};


void
idk::glUBO::unbind()
{
    _offset = 0;
    gl::bindBuffer(GL_UNIFORM_BUFFER, 0);
};


void
idk::glUBO::add(GLintptr size, const void *data)
{
    gl::bufferSubData(GL_UNIFORM_BUFFER, _offset, size, data);
    _offset += idk::roundup(size, 16);
}

