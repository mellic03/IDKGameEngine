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


class idk::RenderEngine
{
private:
    SDL_Window                                  *_SDL_window;
    SDL_GLContext                               _SDL_gl_context;

    idk::ShaderInterface                        _shader_interface;

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
  
    idk::Shader                                 _screenquad_shader;

    idk::stack<GLuint>                          _gltu_available;
    idk::stack<GLuint>                          _gltu_unavailable;

    idk::Allocator<GLuint>                      _texture_allocator;
    idk::Allocator<idk::Material>               _material_allocator;
    idk::Allocator<idk::Model>                  _model_allocator;
    idk::Allocator<idk::transform>              _transform_allocator;
    idk::Allocator<idk::Camera>                 _camera_allocator;

    // [shader_id] = vector<pair<model_id, transform_id>>;
    std::unordered_map<GLuint, idk::vector<idk::pair<int, int>>>  _model_draw_queue;


                                                friend class idk::Engine;
                                                RenderEngine(size_t w, size_t h);


    bool                                        _line_has_include(std::string &line);
    std::string                                 _parse_shader_include(std::string root, std::string line);
    std::string                                 _parse_shader_source(std::string root, std::string glsl);
    GLuint                                      _load_shader(std::string root, std::string vs, std::string fs);
    void                                        _set_uniform_int(const char *, int);
    void                                        _set_uniform_float(const char *, float);
    void                                        _set_uniform_vec2(const char *, idk::vec2 &);
    void                                        _set_uniform_vec3(const char *, idk::vec3 &);
    void                                        _set_uniform_vec4(const char *, idk::vec4 &);
    void                                        _set_uniform_mat3(const char *, idk::mat3 &);
    void                                        _set_uniform_mat4(const char *, idk::mat4 &);

    
    GLuint                                      _gen_texture(std::string path);
    void                                        _bind_texture(GLuint texture_id);
    void                                        _unbind_texture(GLuint texture_id);

    GLuint                                      _pop_gltextureunit_id();
    void                                        _free_gltextureunits();

    void                                        _bind_material(int id);
    void                                        _bind_shader(GLuint shader_id);
    void                                        _draw_mesh(idk::Mesh &mesh);
    void                                        _draw_model(int id, int transform_id);
    void                                        _draw_to_screen(GLuint texture_id);


public:

    int                                         createCamera()              { return _camera_allocator.add();    };
    int                                         createTransform()           { return _transform_allocator.add(); };

    void                                        deleteCamera(int id)        { _camera_allocator.remove(id);      };
    void                                        useCamera(int id)           { _active_camera_id = id;            };
    idk::Camera &                               getCamera(int id)           { return _camera_allocator.get(id);  };

    GLuint                                      compileShaderProgram(std::string root, std::string vs, std::string fs);
    void                                        useShaderProgram(int id)    { _active_shader_id = id; };


    int                                         loadOBJ(std::string root, std::string obj, std::string mtl);
    void                                        drawModel(int model_id, int transform_id);

    void                                        beginFrame();
    void                                        endFrame();

};

