#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <assert.h>

// #define IDK_DEBUG

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