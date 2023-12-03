#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <set>
#include <map>
#include <vector>
#include <string>


#define assertm(exp, msg) assert(((void)msg, exp))


#define GLClearError() \
{ \
  while (glGetError() != GL_NO_ERROR); \
}

#ifdef IDK_DEBUG
  #define GLCALL(glFunc) \
  { \
    GLClearError(); \
    glFunc \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) \
    { \
        printf("OpenGL Error: %s\n", gluErrorString(err)); \
        fflush(stdout); \
        assert(err == GL_NO_ERROR); \
    } \
  }
#else
  #define GLCALL(glFunc) \
  { \
    glFunc \
  }
#endif


