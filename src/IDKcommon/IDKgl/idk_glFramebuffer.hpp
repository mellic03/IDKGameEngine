#pragma once

#include "../idk_utility.h"
#include "../idk_glcall.h"
#include "../idk_datastructures/idk_vector.h"

namespace idk
{
    struct ColorAttachmentConfig;
    struct DepthAttachmentConfig;
    class  glFramebuffer;
};



struct idk::ColorAttachmentConfig
{
    GLint  internalformat = GL_RGBA16F;
    GLenum format         = GL_RGBA;
    GLenum minfilter      = GL_NEAREST;
    GLenum magfilter      = GL_LINEAR;
    GLenum wrap_s         = GL_CLAMP_TO_EDGE;
    GLenum wrap_t         = GL_CLAMP_TO_EDGE;
    GLenum datatype       = GL_FLOAT;
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
    GLuint m_FBO, m_RBO;
    idk::vector<GLuint> m_gl_attachments;

public:
    idk::vector<GLuint> attachments;
    GLuint              depth_attachment;

    void    reset( int w, int h, size_t num_attachments );
    void    colorAttachment( int idx, const idk::ColorAttachmentConfig &config );
    void    depthAttachment( const idk::DepthAttachmentConfig &config );
    void    bind();
    void    unbind();
    void    clear( GLbitfield mask );

};

