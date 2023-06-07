#pragma once

#include <unordered_map>

#include "IDKgraphics_common/IDKgraphics_common.h"
#include "IDK_glShader.h"
#include "IDK_glInterface.h"
#include "IDK_camera.h"
#include "model.h"


#define modelqueue_t std::unordered_map<GLuint, idk::vector<idk::pair<uint, uint>>>


class idk::RenderEngine
{
private:
    SDL_Window                                  *_SDL_window;
    SDL_GLContext                               _SDL_gl_context;

    idk::glInterface                            _gl_interface;

    GLuint                                      _active_shader_id;
    int                                         _active_camera_id;

    GLuint                                      _quad_FBO;
    GLuint                                      _quad_VBO;
    GLuint                                      _quad_VAO;
    GLuint                                      _quad_texture;

    float _quad_vertices[30] = {
      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
      -1.0f, -1.0f,  -0.999f,  0.0f,  0.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,

      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,
       1.0f,  1.0f,  -0.999f,  1.0f,  1.0f
    };

    idk::glInterface::ScreenBuffer      _gbuffer_geometrypass;
    GLuint                              _gbuffer_geometrypass_shader;

    idk::glInterface::ScreenBuffer      _screenquad_buffer;
    GLuint                              _screenquad_shader;
    GLuint                              _screenquad_shader2;

    uint                                            _active_glShader_id;
    std::unordered_map<GLuint, idk::glShader>       _glShader_allocator;

    idk::Allocator<GLuint>              _texture_allocator;
    idk::Allocator<idk::Material>       _material_allocator;
    idk::Allocator<idk::Model>          _model_allocator;
    idk::Allocator<idk::transform>      _transform_allocator;
    idk::Allocator<idk::Camera>         _camera_allocator;

                                        // queue[shader_id] = vector<{model_id, transform_id}>;
    modelqueue_t                        _model_draw_queue;


                                        friend class idk::Engine;
                                        RenderEngine(size_t w, size_t h);
    void                                _init_SDL_OpenGL(size_t w, size_t h);
    void                                _init_screenquad();


public:
    idk::glInterface &                  glInterface()               { return _gl_interface;              };
    uint                                createTransform()           { return _transform_allocator.add(); };
    idk::transform &                    getTransform(uint id)       { return _transform_allocator.get(id); };

    uint                                createCamera()              { return _camera_allocator.add();    };
    void                                deleteCamera(int id)        { _camera_allocator.remove(id);      };
    void                                useCamera(int id)           { _active_camera_id = id;            };
    idk::Camera &                       getCamera(int id)           { return _camera_allocator.get(id);  };

    GLuint                              compileShaderProgram(std::string root, std::string vs, std::string fs);
    void                                useShaderProgram(GLuint id)    { _active_shader_id = id; };
    glShader &                          activeShader()  { return _glShader_allocator[_active_glShader_id]; };

    uint                                loadOBJ(std::string root, std::string obj, std::string mtl);
    void                                drawModel(uint model_id, uint transform_id);

    void                                beginFrame();
    void                                endFrame();

};


#undef modelqueue_t