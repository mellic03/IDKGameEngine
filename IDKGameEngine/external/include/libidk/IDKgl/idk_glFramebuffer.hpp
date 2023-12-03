#pragma once

#include "common.hpp"
#include <vector>

namespace idk
{
    struct glColorConfig;
    struct DepthAttachmentConfig;
    class  glFramebuffer;
};



struct idk::glColorConfig
{
    GLint  internalformat = GL_RGBA16F;
    GLenum format         = GL_RGBA;
    GLenum minfilter      = GL_NEAREST;
    GLenum magfilter      = GL_LINEAR;
    GLenum wrap_s         = GL_CLAMP_TO_EDGE;
    GLenum wrap_t         = GL_CLAMP_TO_EDGE;
    GLenum wrap_r         = GL_CLAMP_TO_EDGE;
    GLenum datatype       = GL_FLOAT;
    bool   genmipmap      = true;
    bool   setmipmap      = false;
    GLint  texbaselevel   = 0;
    GLint  texmaxlevel    = 0;
};


struct idk::DepthAttachmentConfig
{
    GLint  internalformat = GL_DEPTH_COMPONENT16;
    GLenum datatype       = GL_FLOAT;
};


class idk::glFramebuffer
{
private:
    bool m_first = true;
    glm::ivec2 m_size;
    std::vector<GLuint> m_gl_attachments;

public:
    GLuint m_FBO, m_RBO;
    std::vector<GLuint> attachments;
    GLuint              cubemap_attachment;
    GLuint              depth_attachment;

    void    reset( int w, int h, size_t num_attachments );
    void    cubemapColorAttachment( const idk::glColorConfig &config );
    void    colorAttachment( int idx, const idk::glColorConfig &config );
    void    depthAttachment( const idk::DepthAttachmentConfig &config );
    void    generateMipmap( int idx );
    void    generateMipmapCube();
    void    viewport(int x, int y, int w, int h);
    void    bind();
    void    unbind();
    void    clear( GLbitfield mask );

};

