#pragma once


#include "IDKcommon/IDKcommon.h"

namespace idk
{
enum class UBOloc: GLuint
{
    GLOBAL_MATRICES     = 2,
    GLOBAL_POINTLIGHTS  = 3,
    GLOBAL_SPOTLIGHTS   = 4
};
};


namespace idk::gl
{
    inline void                    enable(GLenum cap)      { GLCALL( glEnable(cap);  ) };
    inline void                    disable(GLenum cap)     { GLCALL( glDisable(cap); ) };
    inline void                    enable()  {  };
    inline void                    disable() {  };

    template <typename... GLenums> inline void     enable( GLenum, GLenums... );
    template <typename... GLenums> inline void     disable( GLenum, GLenums... );

    // glGenXXX ------------------------------------------------------------------------------
    /**/
    void genVertexArrays( GLsizei n, GLuint *arrays );
    void genBuffers( GLsizei n, GLuint *buffers );
    void genTextures( GLsizei n, GLuint *textures );
    // ---------------------------------------------------------------------------------------

    // glDeleteXXX ---------------------------------------------------------------------------
    /**/
    void deleteVertexArrays( GLsizei n, GLuint *arrays );
    void deleteBuffers( GLsizei n, GLuint *buffers );
    void deleteTextures( GLsizei n, GLuint *textures );
    // ---------------------------------------------------------------------------------------

    // glBindXXX -----------------------------------------------------------------------------
    /**/
    void bindVertexArray( GLuint VAO );
    void bindBuffer( GLenum type, GLuint buf );
    void bindBufferBase( GLenum target, GLuint index, GLuint buffer );
    void bindBufferBase( GLenum target, UBOloc loc, GLuint buffer );
    void bindFramebuffer( GLenum target, GLint framebuffer );
    void bindTexture( GLenum target, GLuint texture );
    // ---------------------------------------------------------------------------------------

    // glDrawXXX -----------------------------------------------------------------------------
    /**/
    void drawArrays( GLenum mode, GLint first, GLsizei count );
    void drawElements( GLenum mode, GLsizei count, GLenum type, const void *indices );
    // ---------------------------------------------------------------------------------------

    // glBufferXXX ---------------------------------------------------------------------------
    /**/
    void bufferData( GLenum target, GLsizeiptr size, const void *data, GLenum usage );
    void bufferSubData( GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
    // ---------------------------------------------------------------------------------------

    // glActiveXXX ---------------------------------------------------------------------------
    /**/
    void activeTexture( GLenum texture );
    // ---------------------------------------------------------------------------------------

    // Uniforms ------------------------------------------------------------------------------
    /**/
    GLint getUniformLocation( GLuint program, std::string name );
    // ---------------------------------------------------------------------------------------

    // Uniforms ------------------------------------------------------------------------------
    GLint getUniformLocation( GLuint program, std::string name );
    // ---------------------------------------------------------------------------------------

    void vertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset );
    void enableVertexAttribArray( GLuint index );

    void useProgram( GLuint program );
    void clearColor( float r, float g, float b, float a );
    void clear( GLbitfield mask );
    void viewport( GLint x, GLint y, GLsizei w, GLsizei h );
};


template <typename... GLenums>
inline void
idk::gl::enable( GLenum cap, GLenums... rest )
{
    enable(cap);
    enable(rest...);
}


template <typename... GLenums>
inline void
idk::gl::disable( GLenum cap, GLenums... rest )
{
    disable(cap);
    disable(rest...);
}
