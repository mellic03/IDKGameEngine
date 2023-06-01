#pragma once

#include <unordered_map>

#include "../IDKcore/IDKcore.h"
#include "shader.h"
#include "model.h"
#include "camera.h"


class IDK::RenderEngine
{
private:
    SDL_Window *_SDL_window;
    SDL_GLContext _SDL_gl_context;

    GLuint _active_shader_id;
    int _active_camera_id;

    GLuint _quad_FBO, _quad_VBO, _quad_VAO, _quad_texture;
    float _quad_vertices[30] = {
      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
      -1.0f, -1.0f,  -0.999f,  0.0f,  0.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,

      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,
       1.0f,  1.0f,  -0.999f,  1.0f,  1.0f
    };
  
    IDK::Shader _screenquad_shader;


    IDK::Allocator<GLuint> _gltextureunit_allocator;
    IDK::Allocator<IDK::Texture> _texture_allocator;
    IDK::Allocator<IDK::Material> _material_allocator;
    IDK::Allocator<IDK::Model> _model_allocator;
    IDK::Allocator<IDK::Camera> _camera_allocator;


    using it_pair = std::pair<int, IDK::transform *>;
    std::unordered_map<GLuint, Shader> _shaders;
    std::unordered_map<GLuint, IDK::vector<it_pair>> _model_queue;

    friend class IDK::Engine;
    RenderEngine(size_t w, size_t h);

    void _use_shader(Shader &shader);
    void _draw_model(Model *model, IDK::transform *transform);
    void _draw_model(int id, IDK::transform *transform);
    void _draw_to_screen(GLuint texture_id);


public:
    ~RenderEngine();

    void addShader(Shader &shader);

    void useCamera(int camera_id)  { _active_camera_id = camera_id;    };
    int createCamera()             { return _camera_allocator.add();   };
    void deleteCamera(int id)      { _camera_allocator.remove(id);     };
    IDK::Camera &getCamera(int id) { return _camera_allocator.get(id); };

    void useShader(int id);
    void drawModel(int id, IDK::transform &transform);

    void beginFrame();
    void endFrame();

};

