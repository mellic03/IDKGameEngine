#include "idk_gltools.hpp"
#include <fstream>
#include <sstream>
#include <iostream>



GLuint
idk::gltools::loadTexture( size_t w, size_t h, void *data, bool srgb, GLint minfilter, GLint magfilter )
{
    GLuint texture_id;

    gl::genTextures(1, &texture_id);
    gl::bindTexture(GL_TEXTURE_2D, texture_id);
    // gl::pixelStorei(GL_UNPACK_ALIGNMENT, 1);

    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, magfilter);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLint internalformat = (srgb) ? GL_SRGB8 : GL_RGB8;
    gl::texImage2D(GL_TEXTURE_2D, 0, internalformat, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    gl::generateMipmap(GL_TEXTURE_2D);

    gl::bindTexture(GL_TEXTURE_2D, 0);

    return texture_id;
}



GLuint
idk::gltools::loadTexture( std::string filepath, bool srgb, GLint minfilter, GLint magfilter )
{
    SDL_Surface      *tmp    = IMG_Load(filepath.c_str());
    SDL_PixelFormat  *target = SDL_AllocFormat(SDL_PIXELFORMAT_RGB24);
    SDL_Surface      *img    = SDL_ConvertSurface(tmp, target, 0);

    GLuint texture_id = gltools::loadTexture(img->w, img->h, (uint32_t *)(img->pixels), srgb, minfilter, magfilter);

    SDL_FreeFormat(target);
    SDL_FreeSurface(tmp);
    SDL_FreeSurface(img);

    return texture_id;
}



GLuint
idk::gltools::loadCubemap( std::string root, std::vector<std::string> filenames, glColorConfig config )
{
    GLuint texture_id;

    gl::genTextures(1, &texture_id);
    gl::bindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    for (int i=0; i<6; i++)
    {
        std::string       filepath = root + filenames[i];
        SDL_Surface      *tmp      = IMG_Load(filepath.c_str());
        SDL_PixelFormat  *target   = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
        SDL_Surface      *img      = SDL_ConvertSurface(tmp, target, 0);

        gl::texImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, config.internalformat, img->w, img->h, 0, config.format,
            config.datatype, (void *)(img->pixels)
        );

        SDL_FreeFormat(target);
        SDL_FreeSurface(tmp);
        SDL_FreeSurface(img);
    }

    gl::texParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, config.minfilter);
    gl::texParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, config.magfilter);
    gl::texParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl::texParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl::texParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if (config.genmipmap)
    {
        gl::generateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    else if (config.setmipmap)
    {
        gl::texParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, config.texbaselevel);
        gl::texParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL,  config.texmaxlevel);
    }

    return texture_id;
}


void
idk::gltools::loadCubemapMip( std::string directory, std::vector<std::string> filenames,
                              glColorConfig config, GLuint cubemap, GLint level )
{
    gl::bindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

    for (GLuint face=0; face<6; face++)
    {
        std::string       filepath = directory + std::to_string(level) + filenames[face];
        // std::cout << "loading level " << level << ": " << filepath << "\n";

        SDL_Surface      *tmp    = IMG_Load(filepath.c_str());
        SDL_PixelFormat  *target = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
        SDL_Surface      *img    = SDL_ConvertSurface(tmp, target, 0);

        gl::texImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 
            level, config.internalformat, img->w, img->h, 0, config.format,
            config.datatype, (void *)(img->pixels)
        );

        SDL_FreeFormat(target);
        SDL_FreeSurface(tmp);
        SDL_FreeSurface(img);
    }

}


