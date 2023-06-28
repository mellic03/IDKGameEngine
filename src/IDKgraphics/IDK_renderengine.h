#pragma once

#include <unordered_map>

#include "IDKgraphics_common/IDKgraphics_common.h"
#include "IDK_glUniforms.h"
#include "IDK_glInterface.h"
#include "IDK_camera.h"
#include "IDK_lightsource.h"
#include "model.h"
#include "primitives/primitives.h"


#define modelqueue_t std::unordered_map<GLuint, idk::vector<idk::triple<uint, idk::Transform, idk::glUniforms>>>

class idk::RenderEngine
{
private:
    int                                 _screen_width;
    int                                 _screen_height;

    SDL_Window                          *_SDL_window;
    SDL_GLContext                       _SDL_gl_context;

    glInterface                         _gl_interface;

    glInterface::ScreenBuffer           _gbuffer_geometrypass;
    GLuint                              _gbuffer_geometrypass_shader;

    glInterface::ScreenBuffer           _screenquad_buffer;
    glInterface::ScreenBuffer           _colorgrade_buffer;
    GLuint                              _screenquad_shader;
    GLuint                              _colorgrade_shader;
    GLuint                              _fxaa_shader;

    uint                                _active_camera_id;
    uint                                _active_shader_id;
    glUniforms *                        _active_glUniforms;

    GLuint                              _quad_FBO;
    GLuint                              _quad_VBO;
    GLuint                              _quad_VAO;
    GLuint                              _quad_texture;


    float _quad_vertices[30] = {
      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
      -1.0f, -1.0f,  -0.999f,  0.0f,  0.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,

      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,
       1.0f,  1.0f,  -0.999f,  1.0f,  1.0f
    };

    Allocator<Model>                    _model_allocator;
    Allocator<Transform>                _transform_allocator;
    Allocator<Camera>                   _camera_allocator;
    Allocator<lightsource::Point>       _pointlight_allocator;
    Allocator<lightsource::Spot>        _spotlight_allocator;
    Allocator<lightsource::Dir>         _dirlight_allocator;


    modelqueue_t                        _model_draw_queue;
    modelqueue_t                        _wireframe_draw_queue;

                                        friend class idk::Engine;
                                        RenderEngine(size_t w, size_t h);
    void                                _init_SDL_OpenGL(size_t w, size_t h);
    void                                _init_screenquad();
    void                                _render_screenquad();
    void                                _colorgrade_screenquad();
    void                                _fxaa_screenquad();

public:
    GLuint                              primitive_shader;
    GLuint                              wireframe_shader;
    uint                                SPHERE_PRIMITIVE;

    glInterface &                       glinterface()               { return _gl_interface;             };
   
    uint                                createCamera();
    void                                deleteCamera( uint id )     { _camera_allocator.remove(id);     };
    idk::Camera &                       getCamera( uint id)         { return _camera_allocator.get(id); };
    void                                setActiveCamera( uint id )  { _active_camera_id = id;           };
    idk::Camera &                       getActiveCamera()           { return _camera_allocator.get(_active_camera_id); };

    uint                                createPointLight();
    Allocator<lightsource::Point> &     pointlights()               { return _pointlight_allocator; };

    void                                loadSpherePrimitive( std::string path );
    void                                loadTextures( std::string root );
    uint                                loadOBJ( std::string root, std::string obj, std::string mtl );
    void                                bindShader( GLuint shader_id )  { _active_shader_id = shader_id; };
    void                                drawModel( uint model_id, Transform &transform );
    void                                drawModel( GLuint shader_id, uint primitive_id, Transform &transform );
    void                                drawWireframe( GLuint shader_id, uint primitive_id, Transform &transform );
    void                                setUniform_vec3( GLuint shader_id, std::string name, glm::vec3 v );

    idk::glUniforms &                   glUniformInterface()    { return *_active_glUniforms; };

    void                                beginFrame();
    void                                endFrame();

    int                                 width()  const  { return _screen_width; };
    int                                 height() const  { return _screen_height; };

};


#undef modelqueue_t
