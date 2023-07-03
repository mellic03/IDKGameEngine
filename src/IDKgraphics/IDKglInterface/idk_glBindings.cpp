#include "idk_glBindings.h"



// glGenXXX ------------------------------------------------------------------------------
void idk::gl::genVertexArrays( GLsizei n, GLuint *arrays )
{
    GLCALL( glGenVertexArrays(n, arrays); )
}

void idk::gl::genBuffers( GLsizei n, GLuint *buffers )
{
    GLCALL( glGenBuffers(n, buffers); )
}

void idk::gl::genTextures( GLsizei n, GLuint *textures )
{
    GLCALL( glGenTextures(n, textures); )
}
// ---------------------------------------------------------------------------------------


// glDeleteXXX ---------------------------------------------------------------------------
void idk::gl::deleteVertexArrays( GLsizei n, GLuint *arrays )
{
    GLCALL( glDeleteVertexArrays(n, arrays); )
}

void idk::gl::deleteBuffers( GLsizei n, GLuint *buffers )
{
    GLCALL( glDeleteBuffers(n, buffers); )
}

void idk::gl::deleteTextures( GLsizei n, GLuint *textures )
{
    GLCALL( glDeleteTextures(n, textures); )
}
// ---------------------------------------------------------------------------------------


// glBindXXX -----------------------------------------------------------------------------
void idk::gl::bindVertexArray( GLuint VAO )
{
    GLCALL( glBindVertexArray(VAO); )
}

void idk::gl::bindBuffer( GLenum type, GLuint buf )
{
    GLCALL( glBindBuffer(type, buf); )
}

void idk::gl::bindFramebuffer( GLenum target, GLint framebuffer )
{
    GLCALL( glBindFramebuffer(target, framebuffer); )
}

// ---------------------------------------------------------------------------------------

// glDrawXXX -----------------------------------------------------------------------------
void idk::gl::drawArrays( GLenum mode, GLint first, GLsizei count )
{
    GLCALL( glDrawArrays(mode, first, count); )
}

void idk::gl::drawElements( GLenum mode, GLsizei count, GLenum type, const void *indices )
{
    GLCALL( glDrawElements(mode, count, type, indices); )
}
// ---------------------------------------------------------------------------------------


void
idk::gl::bufferData( GLenum target, GLsizeiptr size, const void *data, GLenum usage )
{
    GLCALL( glBufferData(target, size, data, usage); )
}


void
idk::gl::vertexAttribPointer( GLuint index, GLint size,
                              GLenum type, GLboolean normalized,
                              GLsizei stride, GLuint offset )
{
    GLCALL( glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<void *>(offset)); )
}


void
idk::gl::enableVertexAttribArray( GLuint index )
{
    GLCALL( glEnableVertexAttribArray(index); )
}