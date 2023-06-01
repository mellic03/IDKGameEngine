#pragma once

#include <unordered_map>

#include "../IDKcore/IDKcore.h"
#include "shader.h"
#include "model.h"
#include "camera.h"


class IDK::Renderer
{
private:
    SDL_Window *_SDL_window;
    SDL_GLContext _SDL_gl_context;
    GLuint _active_shader_id;

    GLuint _quad_FBO, _quad_VBO, _quad_VAO, _quad_texture;
    float _quad_vertices[30] = {
      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
      -1.0f, -1.0f,  -0.999f,  0.0f,  0.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,

      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,
       1.0f,  1.0f,  -0.999f,  1.0f,  1.0f
    };


    using mt_pair = std::pair<Model *, IDK::transform *>;
    using it_pair = std::pair<int, IDK::transform *>;
    std::unordered_map<GLuint, Shader> _shaders;
    std::unordered_map<GLuint, IDK::vector<mt_pair>> _model_queue;
    std::unordered_map<GLuint, IDK::vector<it_pair>> _model_id_queue;

    IDK::Shader _screenquad_shader;

    friend class IDK::Engine;
    Renderer(size_t w, size_t h);

    void _use_shader(Shader &shader);
    void _draw_model(Model *model, IDK::transform *transform);
    void _draw_model(int id, IDK::transform *transform);
    void _draw_to_screen(GLuint texture_id);


public:
    ~Renderer();

    void addShader(Shader &shader);


    /**
     * Switch to the perspective of another camera.
     * @param camera The camera to switch to.
    */
    void useCamera(IDK::Camera &camera);

    /*@param shader The shader to use for all subsequent draw calls.*/
    void useShader(IDK::Shader &shader);
    /*@param shader The shader to use for all subsequent draw calls.*/
    void useShader(GLuint shader_id);

    /**
     * Draw a model.
    */
    void drawModel(IDK::Model &model, IDK::transform &transform);
    void drawModel(int id, IDK::transform &transform);

    void beginFrame();
    void endFrame();

};

