#pragma once

#include <unordered_map>

#include "../IDKcore/IDKcore.h"
#include "model.h"
#include "camera.h"
#include "IDK_glInterface.h"


#define modelqueue_t std::unordered_map<GLuint, idk::vector<idk::pair<int, int>>>


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
  
    GLuint                              _screenquad_shader;

    idk::Allocator<GLuint>              _texture_allocator;
    idk::Allocator<idk::Material>       _material_allocator;
    idk::Allocator<idk::Model>          _model_allocator;
    idk::Allocator<idk::transform>      _transform_allocator;
    idk::Allocator<idk::Camera>         _camera_allocator;

                                        // queue[shader_id] = vector<{model_id, transform_id}>;
    modelqueue_t                        _model_draw_queue;


                                        friend class idk::Engine;
                                        RenderEngine(size_t w, size_t h);

    void                                _draw_to_screen(GLuint texture_id);


public:
    idk::glInterface &                  glInterface()               { return _gl_interface;              };
    uint                                createTransform()           { return _transform_allocator.add(); };

    uint                                createCamera()              { return _camera_allocator.add();    };
    void                                deleteCamera(int id)        { _camera_allocator.remove(id);      };
    void                                useCamera(int id)           { _active_camera_id = id;            };
    idk::Camera &                       getCamera(int id)           { return _camera_allocator.get(id);  };

    GLuint                              compileShaderProgram(std::string root, std::string vs, std::string fs);
    void                                useShaderProgram(int id)    { _active_shader_id = id; };


    uint                                loadOBJ(std::string root, std::string obj, std::string mtl);
    void                                drawModel(int model_id, int transform_id);

    void                                beginFrame();
    void                                endFrame();

};


#undef modelqueue_t
