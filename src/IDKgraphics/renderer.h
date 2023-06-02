#pragma once

#include <unordered_map>

#include "../IDKcore/IDKcore.h"
#include "shader.h"
#include "model.h"
#include "camera.h"


class TextureUnitAllocator
{
private:

public:

};


class IDK::RenderEngine
{
private:
    SDL_Window                                  *_SDL_window;
    SDL_GLContext                               _SDL_gl_context;

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
  
    IDK::Shader                                 _screenquad_shader;


    IDK::Allocator<GLuint>                      _shader_allocator;
    IDK::Allocator<GLuint>                      _texture_allocator;
    IDK::Allocator<IDK::Material>               _material_allocator;
    IDK::Allocator<IDK::Model>                  _model_allocator;
    IDK::Allocator<IDK::Camera>                 _camera_allocator;


                                                friend class IDK::Engine;
                                                RenderEngine(size_t w, size_t h);


    GLuint                                      _load_shader(std::string root, std::string vs, std::string fs);

    void                                        _load_textures(std::string path);
    GLuint                                      _gen_texture(std::string path);

    void                                        _bind_texture(int id);
    void                                        _bind_material(int id);
    void                                        _use_shader(Shader &shader);
    void                                        _draw_mesh(IDK::Mesh &mesh);
    void                                        _draw_model(int id, IDK::transform *transform);
    void                                        _draw_to_screen(GLuint texture_id);


public:


    int                                         createCamera()                  { return _camera_allocator.add();   };
    void                                        deleteCamera(int id)            { _camera_allocator.remove(id);     };
    void                                        useCamera(int id)               { _active_camera_id = id;           };
    IDK::Camera                                 &getCamera(int id)              { return _camera_allocator.get(id); };

    void                                        loadShader(std::string path);
    void                                        useShader(int id);
    void                                        drawModel(int id, IDK::transform &transform);

    void                                        beginFrame();
    void                                        endFrame();

};

