#pragma once

#include "IDKcommon/IDKcommon.h"

namespace idk {
    struct glFramebuffer;
    struct ColorAttachmentConfig;
    struct DepthAttachmentConfig;
    class glFramebufferNew;
};


struct idk::glFramebuffer
{
    int width  = 0;
    int height = 0;
    GLuint FBO = 0;
    GLuint RBO = 0;
    std::vector<GLuint> output_textures;
};



struct idk::ColorAttachmentConfig
{
    GLint  internalformat;
    GLenum minfilter;
    GLenum magfilter;
    GLenum type;
};


struct idk::DepthAttachmentConfig
{
    GLint  internalformat;
    GLenum minfilter;
    GLenum magfilter;
    GLenum type;
};


class idk::glFramebufferNew
{
private:


public:
    glm::ivec2 size;
    GLuint FBO, RBO;
    std::vector<GLuint> attachments;

            glFramebufferNew( int w, int h );

    void    colorAttachment( const idk::ColorAttachmentConfig &config );
    void    depthAttachment();

};

