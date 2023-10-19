#pragma once

#include "IDKcommon/IDKcommon.h"

namespace idk {
    struct ColorAttachmentConfig;
    struct DepthAttachmentConfig;
    class glFramebuffer;
};



struct idk::ColorAttachmentConfig
{
    GLint  internalformat;
    GLenum minfilter;
    GLenum magfilter;
    GLenum datatype;
};


struct idk::DepthAttachmentConfig
{
    GLint  internalformat;
    GLenum minfilter;
    GLenum magfilter;
    GLenum datatype;
};


class idk::glFramebuffer
{
private:
    glm::ivec2 m_size;
    GLuint m_FBO, m_RBO;

public:
    std::vector<GLuint> attachments;

    void    reset( int w, int h, size_t num_attachments );
    void    colorAttachment( int idx, const idk::ColorAttachmentConfig &config );
    void    depthAttachment();
    void    bind();
    void    unbind();
    void    clear( GLbitfield mask );

};

