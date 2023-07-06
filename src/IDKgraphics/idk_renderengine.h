#pragma once

#include <unordered_map>
#include "IDKcommon/IDKcommon.h"

#include "idk_model_manager.h"
#include "IDKglInterface/IDKglInterface.h"

#include "idk_camera.h"
#include "idk_lightsource.h"
#include "primitives/primitives.h"


namespace idk { class RenderEngine; };

#define modelqueue_t std::unordered_map<GLuint, idk::vector<idk::pair<int, idk::Transform>>>

class idk::RenderEngine
{
private:
    int                 _screen_width;
    int                 _screen_height;

    SDL_Window *        _SDL_window;
    SDL_GLContext       _SDL_gl_context;

    // idk::glFramebuffers ------------------------------------
    /***/
    glFramebuffer       _gb_geometry_buffer;
    glFramebuffer       _screenquad_buffer;
    glFramebuffer       _colorgrade_buffer;
    // --------------------------------------------------------

    // Shaders ------------------------------------------------
    /***/
    GLuint              _gb_geometry_shader;
    GLuint              _screenquad_shader;
    GLuint              _colorgrade_shader;
    GLuint              _fxaa_shader;
    // --------------------------------------------------------

    uint                _active_camera_id;
    glUniforms *        _active_glUniforms;

    GLuint              _quad_FBO;
    GLuint              _quad_VBO;
    GLuint              _quad_VAO;
    GLuint              _quad_texture;


    ModelManager                        _model_manager;
    Allocator<Transform>                _transform_allocator;
    Allocator<Camera>                   _camera_allocator;
    Allocator<lightsource::Point>       _pointlight_allocator;
    Allocator<lightsource::Spot>        _spotlight_allocator;
    Allocator<lightsource::Dir>         _dirlight_allocator;


    modelqueue_t        _model_draw_queue;
    modelqueue_t        _wireframe_draw_queue;


    // Initialization ---------------------------------------------------------------------
    /***/
    void                _init_SDL_OpenGL( size_t w, size_t h );
    void                _init_screenquad();
    // ------------------------------------------------------------------------------------


    // Draw-related methods ---------------------------------------------------------------
    /***/
    void                _render_screenquad( GLuint shader, glFramebuffer &in, glFramebuffer &out );
    void                _render_screenquad( GLuint shader, glFramebuffer &in );

    void                _bind_material( idk::Material & );
    void                _draw_model( idk::Model &, idk::Transform & );
    void                _draw_wireframe( idk::Model &, idk::Transform & );
    // ------------------------------------------------------------------------------------


public:

    // Built-in shaders -------------------------------------------------------------------
    /***/
    GLuint              solid_shader;
    GLuint              wireframe_shader;
    // ------------------------------------------------------------------------------------

    // Built-in primitives ----------------------------------------------------------------
    /***/
    GLuint              SPHERE_PRIMITIVE;
    GLuint              CUBE_PRIMITIVE;
    // ------------------------------------------------------------------------------------

                                        RenderEngine(size_t w, size_t h);

    int                                 createCamera();
    idk::Camera &                       getCamera()         { return _camera_allocator.get(_active_camera_id); };

    int                                 createSpotlight();
    int                                 createPointlight();
    Allocator<lightsource::Point> &     pointlights()       { return _pointlight_allocator; };
    Allocator<lightsource::Spot> &      spotlights()        { return _spotlight_allocator; };


    // Models -----------------------------------------------------------------------------
    /***/
    ModelManager &                      modelManager()  { return _model_manager; };

    void                                drawModel( GLuint shader_id, int model_id, Transform &transform );
    void                                drawWireframe( GLuint shader_id, int model_id, Transform &transform );
    // ------------------------------------------------------------------------------------


    void                                beginFrame();
    void                                endFrame();
    void                                resize( int w, int h );

    int                                 width()  const  { return _screen_width; };
    int                                 height() const  { return _screen_height; };

};

#undef modelqueue_t

