#include "idk_glSSBO.hpp"


void
idk::glSSBO::init()
{
    gl::genBuffers(1, &m_SSBO);
}


void
idk::glSSBO::bufferData( size_t nbytes, void *data )
{
    gl::bindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
    gl::bufferData(GL_SHADER_STORAGE_BUFFER, nbytes, data, GL_DYNAMIC_COPY);
    gl::bindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_SSBO);
    gl::bindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}


void
idk::glSSBO::bufferSubData( size_t offset, size_t nbytes, void *data )
{
    gl::bindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
    gl::bufferSubData(GL_SHADER_STORAGE_BUFFER, offset, nbytes, data);
    gl::bindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}


