#include "idk_glFramebuffer.hpp"
#include "idk_glBindings.hpp"


void
idk::glFramebuffer::reset( int w, int h, size_t num_attachments )
{
    m_size.x = w;  m_size.y = h;
    m_gl_attachments.resize(0);
    attachments.resize(num_attachments);

    if (m_first == false)
    {
        gl::deleteFramebuffers(1, &m_FBO);
        gl::deleteRenderbuffers(1, &m_RBO);
    }

    gl::genFramebuffers(1, &m_FBO);
    gl::genRenderbuffers(1, &m_RBO);

    gl::bindRenderbuffer(GL_RENDERBUFFER, m_FBO);
    gl::bindFramebuffer(GL_FRAMEBUFFER, m_RBO);

    GLCALL( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h); )
    GLCALL( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO); )

    gl::bindRenderbuffer(GL_RENDERBUFFER, 0);
    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);


    m_first = false;
}


void
idk::glFramebuffer::colorAttachment( int idx, const idk::ColorAttachmentConfig &config )
{
    gl::bindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    gl::deleteTextures(1, &attachments[idx]);
    gl::genTextures(1, &attachments[idx]);
    gl::bindTexture(GL_TEXTURE_2D, attachments[idx]);

    gl::texImage2D(
        GL_TEXTURE_2D, 0, config.internalformat,
        m_size.x, m_size.y, 0, config.format, config.datatype, NULL
    );

    if (config.format != GL_RGBA_INTEGER)
    {
        gl::generateMipmap(GL_TEXTURE_2D);
        gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minfilter);
        gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magfilter);
        gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     config.wrap_s);
        gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     config.wrap_t);
    }


    gl::framebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+idx, GL_TEXTURE_2D, attachments[idx], 0
    );

    m_gl_attachments.push_back(GL_COLOR_ATTACHMENT0 + idx);
    glDrawBuffers(m_gl_attachments.size(), &(m_gl_attachments[0]));

    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);
    gl::bindTexture(GL_TEXTURE_2D, 0);
}


void
idk::glFramebuffer::depthAttachment( const idk::DepthAttachmentConfig &config )
{
    gl::bindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    gl::deleteTextures(1, &depth_attachment);
    gl::genTextures(1, &depth_attachment);
    gl::bindTexture(GL_TEXTURE_2D, depth_attachment);

    gl::texImage2D(
        GL_TEXTURE_2D, 0, config.internalformat,
        m_size.x, m_size.y, 0, GL_DEPTH_COMPONENT, config.datatype, NULL
    );

    m_gl_attachments.push_back(GL_DEPTH_ATTACHMENT);
    glDrawBuffers(m_gl_attachments.size(), &(m_gl_attachments[0]));

    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);
    gl::bindTexture(GL_TEXTURE_2D, 0);
}


void
idk::glFramebuffer::bind()
{
    gl::bindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    gl::viewport(0, 0, m_size.x, m_size.y);
}


void
idk::glFramebuffer::unbind()
{
    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);
    gl::viewport(0, 0, m_size.x, m_size.y);
}


void
idk::glFramebuffer::clear( GLbitfield mask )
{
    this->bind();
    gl::clearColor(0.0f, 1.0f, 0.0f, 0.0f);
    gl::clear(mask); 
}


