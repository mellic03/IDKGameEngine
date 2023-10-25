#include "idk_glSSBO.hpp"


void
idk::glSSBO::init( GLuint index )
{
    gl::genBuffers(1, &m_SSBO);
    gl::bindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_SSBO);
}


void
idk::glSSBO::bufferData( size_t nbytes, void *data )
{
    gl::bindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
    gl::bufferData(GL_SHADER_STORAGE_BUFFER, nbytes, data, GL_DYNAMIC_READ);
    gl::bindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}


void
idk::glSSBO::bufferSubData( size_t offset, size_t nbytes, void *data )
{
    gl::bindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
    gl::bufferSubData(GL_SHADER_STORAGE_BUFFER, offset, nbytes, data);
    gl::bindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}




void *
idk::glSSBO::mapBuffer()
{
    gl::bindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
    return gl::mapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
}


void
idk::glSSBO::unmapBuffer()
{
    gl::unmapBuffer(GL_SHADER_STORAGE_BUFFER);
    gl::bindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
