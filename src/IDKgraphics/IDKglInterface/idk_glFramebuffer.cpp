#include "idk_glFramebuffer.hpp"
#include "idk_glBindings.hpp"


idk::glFramebufferNew::glFramebufferNew( int w, int h ): size(w, h)
{

}


void
idk::glFramebufferNew::colorAttachment( const idk::ColorAttachmentConfig &config )
{
    GLuint texture;
    gl::genTextures(1, &texture);

    gl::bindFramebuffer(GL_FRAMEBUFFER, FBO);
    gl::bindTexture(GL_TEXTURE_2D, texture);

    gl::texImage2D(
        GL_TEXTURE_2D, 0, config.internalformat, size.x, size.y, 0, GL_RGBA, config.type, NULL
    );

    gl::generateMipmap(GL_TEXTURE_2D);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minfilter);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magfilter);

    attachments.push_back(texture);
}


void
idk::glFramebufferNew::depthAttachment()
{
    
}




// void
// weewa_genIdkFramebuffer(
//     int width, int height, GLuint &FBO, GLuint &RBO,
//     std::vector<GLuint> &textures, GLenum minf = GL_LINEAR, GLenum magf = GL_LINEAR )
// {
//     GLCALL( glDeleteFramebuffers(1, &FBO); )
//     GLCALL( glDeleteRenderbuffers(1, &RBO); )
//     idk::gl::deleteTextures(textures.size(), &(textures[0]));

//     GLCALL( glGenFramebuffers(1, &FBO); )
//     GLCALL( glGenRenderbuffers(1, &RBO); )
//     idk::gl::genTextures(textures.size(), &(textures[0]));

//     idk::gl::bindFramebuffer(GL_FRAMEBUFFER, FBO);

//     for (size_t i=0; i<textures.size(); i++)
//     {
//         idk::gl::bindTexture(GL_TEXTURE_2D, textures[i]);
//         GLCALL( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL); )
//         GLCALL( glGenerateMipmap(GL_TEXTURE_2D); )
//         GLCALL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minf); )
//         GLCALL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magf); )
//         GLCALL( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, textures[i], 0);   )
//     }

//     idk::vector<GLuint> attachments(textures.size());
//     for (size_t i=0; i<textures.size(); i++)
//         attachments[i] = GL_COLOR_ATTACHMENT0 + i;
//     GLCALL( glDrawBuffers(textures.size(), &(attachments[0])); )

//     GLCALL( glBindRenderbuffer(GL_RENDERBUFFER, RBO); )
//     GLCALL( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); )
//     GLCALL( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO); )

//     GLCALL( glBindRenderbuffer(GL_RENDERBUFFER, 0); )
//     idk::gl::bindFramebuffer(GL_FRAMEBUFFER, 0);
// }



// idk::glFramebuffer
// idk::gltools::genIdkFramebuffer(int width, int height, int num_render_targets, GLenum minf, GLenum magf)
// {
//     idk::glFramebuffer fb;
//     fb.width = width;
//     fb.height = height;
//     for (int i=0; i<num_render_targets; i++)
//         fb.output_textures.push_back(0);

//     weewa_genIdkFramebuffer(width, height, fb.FBO, fb.RBO, fb.output_textures, minf, magf);

//     return fb;
// }



