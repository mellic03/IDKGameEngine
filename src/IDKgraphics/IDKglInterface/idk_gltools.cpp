#include "idk_gltools.hpp"
#include <fstream>
#include <sstream>
#include <iostream>



GLuint
idk::gltools::loadTexture( int w, int h, uint32_t *data, bool srgb )
{
    GLuint texture_id;

    gl::genTextures(1, &texture_id);
    gl::bindTexture(GL_TEXTURE_2D, texture_id);

    gl::pixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (srgb)
    {
        gl::texImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    else
    {
        gl::texImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    gl::generateMipmap(GL_TEXTURE_2D);

    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    gl::bindTexture(GL_TEXTURE_2D, 0);

    return texture_id;
}



GLuint
idk::gltools::loadTexture( std::string filepath, bool srgb )
{
    SDL_Surface *img = IMG_Load(filepath.c_str());
    const size_t w = img->w;
    const size_t h = img->h;
    const uint32_t *data = (uint32_t *)(img->pixels);


    GLuint texture_id;

    gl::genTextures(1, &texture_id);
    gl::bindTexture(GL_TEXTURE_2D, texture_id);
    gl::pixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLint internalformat = (srgb) ? GL_SRGB_ALPHA : GL_RGBA;
    gl::texImage2D(GL_TEXTURE_2D, 0, internalformat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    gl::generateMipmap(GL_TEXTURE_2D);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    gl::bindTexture(GL_TEXTURE_2D, 0);


    return texture_id;
}

