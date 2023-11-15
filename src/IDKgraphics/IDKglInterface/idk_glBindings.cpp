#include "idk_glBindings.hpp"



// glGenXXX ------------------------------------------------------------------------------
/**/
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

void idk::gl::genFramebuffers( GLsizei n, GLuint *framebuffers )
{
    GLCALL( glGenFramebuffers(n, framebuffers); )
}

void idk::gl::genRenderbuffers( GLsizei n, GLuint *renderbuffers )
{
    GLCALL( glGenRenderbuffers(n, renderbuffers); )
}
// ---------------------------------------------------------------------------------------


// glDeleteXXX ---------------------------------------------------------------------------
/**/
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

void idk::gl::deleteFramebuffers( GLsizei n, GLuint *framebuffers )
{
    GLCALL( glDeleteFramebuffers(n, framebuffers); )
}

void idk::gl::deleteRenderbuffers( GLsizei n, GLuint *renderbuffers )
{
    GLCALL( glDeleteRenderbuffers(n, renderbuffers); )
}
// ---------------------------------------------------------------------------------------


// glBindXXX -----------------------------------------------------------------------------
/**/
void idk::gl::bindVertexArray( GLuint VAO )
{
    GLCALL( glBindVertexArray(VAO); )
}

void idk::gl::bindBuffer( GLenum type, GLuint buf )
{
    GLCALL( glBindBuffer(type, buf); )
}

void idk::gl::bindBufferBase( GLenum target, GLuint index, GLuint buffer )
{
    glBindBufferBase(target, index, buffer);
}

void idk::gl::bindBufferBase( GLenum target, UBOloc loc, GLuint buffer )
{
    glBindBufferBase(target, static_cast<GLuint>(loc), buffer);
}

void idk::gl::bindFramebuffer( GLenum target, GLuint framebuffer )
{
    GLCALL( glBindFramebuffer(target, framebuffer); )
}

void idk::gl::bindRenderbuffer( GLenum target, GLuint renderbuffer )
{
    GLCALL( glBindRenderbuffer(target, renderbuffer); )
}

void idk::gl::bindTexture( GLenum target, GLuint texture )
{
    GLCALL( glBindTexture(target, texture); )
}
// ---------------------------------------------------------------------------------------


// glDrawXXX -----------------------------------------------------------------------------
/**/
void idk::gl::drawArrays( GLenum mode, GLint first, GLsizei count )
{
    GLCALL( glDrawArrays(mode, first, count); )
}

void idk::gl::drawElements( GLenum mode, GLsizei count, GLenum type, const void *indices )
{
    GLCALL( glDrawElements(mode, count, type, indices); )
}
// ---------------------------------------------------------------------------------------


// glBufferXXX ---------------------------------------------------------------------------
/**/
void
idk::gl::bufferData( GLenum target, GLsizeiptr size, const void *data, GLenum usage )
{
    GLCALL( glBufferData(target, size, data, usage); )
}

void
idk::gl::bufferSubData( GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
{
    GLCALL( glBufferSubData(target, offset, size, data); )
}

void *
idk::gl::mapBuffer( GLenum target, GLenum access )
{
    void *data;
    GLCALL( data = glMapBuffer(target, access); )
    return data;
}

void
idk::gl::unmapBuffer( GLenum target )
{
    GLCALL( glUnmapBuffer(target); )
}
// ---------------------------------------------------------------------------------------


// glActiveXXX ---------------------------------------------------------------------------
/**/
/**/
void
idk::gl::activeTexture( GLenum texture )
{
    GLCALL( glActiveTexture(texture); )
}
// ---------------------------------------------------------------------------------------

// glTexXXX ------------------------------------------------------------------------------
/**/
void
idk::gl::texImage2D( GLenum target, GLint level, GLint internalformat, GLsizei w, GLsizei h,
                     GLint border, GLenum format, GLenum type, const void *data )
{
    GLCALL( glTexImage2D(target, level, internalformat, w, h, border, format, type, data); )
}

void
idk::gl::texImage3D( GLenum target, GLint level, GLint internalformat, GLsizei w, GLsizei h,
                     GLsizei d, GLint border, GLenum format, GLenum type, const void *data )
{
    GLCALL( glTexImage3D(target, level, internalformat, w, h, d, border, format, type, data); )
}

void idk::gl::texParameteri( GLenum target, GLenum pname, GLint param )
{
    GLCALL( glTexParameteri(target, pname, param); )
}

void idk::gl::generateMipmap( GLenum target )
{
    GLCALL( glGenerateMipmap(target); )
}

void idk::gl::pixelStorei( GLenum pname, GLint param )
{
    GLCALL( glPixelStorei(pname, param); )
}

// ---------------------------------------------------------------------------------------



// Framebuffer stuff ---------------------------------------------------------------------
void idk::gl::framebufferTexture2D( GLenum target, GLenum attachment, GLenum textarget,
                                    GLuint texture, GLint level )
{
    GLCALL( glFramebufferTexture2D(target, attachment, textarget, texture, level); )
}
// ---------------------------------------------------------------------------------------



// Uniforms ------------------------------------------------------------------------------
/**/
GLint
idk::gl::getUniformLocation( GLuint program, std::string name )
{
    return glGetUniformLocation(program, name.c_str());
}


void idk::gl::uniform1i( GLint loc, int i )
{
    GLCALL( glUniform1i(loc, i); )
}


void idk::gl::uniform1f( GLint loc, float f )
{
    GLCALL( glUniform1f(loc, f); )
}


void idk::gl::uniform2fv( GLint loc, GLsizei count, float *value )
{
    GLCALL( glUniform2fv(loc, count, value); )
}


void idk::gl::uniform3fv( GLint loc, GLsizei count, float *value )
{
    GLCALL( glUniform3fv(loc, count, value); )
}


void idk::gl::uniform4fv( GLint loc, GLsizei count, float *value )
{
    GLCALL( glUniform4fv(loc, count, value); )
}


void idk::gl::uniformMatrix3fv( GLint loc, GLsizei count, GLboolean transpose, float *value )
{
    GLCALL( glUniformMatrix3fv(loc, 1, transpose, value); )
}


void idk::gl::uniformMatrix4fv( GLint loc, GLsizei count, GLboolean transpose, float *value )
{
    GLCALL( glUniformMatrix4fv(loc, 1, transpose, value); )
}
// ---------------------------------------------------------------------------------------


void
idk::gl::vertexAttribPointer( GLuint index, GLint size,
                              GLenum type, GLboolean normalized,
                              GLsizei stride, GLuint offset )
{
    GLCALL(
        glVertexAttribPointer(
            index, size, type, normalized,
            stride, reinterpret_cast<void *>(offset)
        );
    )
}


void
idk::gl::enableVertexAttribArray( GLuint index )
{
    GLCALL( glEnableVertexAttribArray(index); )
}


void
idk::gl::useProgram( GLuint program )
{
    GLCALL( glUseProgram(program); )
}


void
idk::gl::clearColor( float r, float g, float b, float a )
{
    GLCALL( glClearColor(r, g, b, a); )
}


void idk::gl::clear( GLbitfield mask )
{
    GLCALL( glClear(mask); )
}


void
idk::gl::viewport( GLint x, GLint y, GLsizei w, GLsizei h )
{
    GLCALL( glViewport(x, y, w, h); )
}


