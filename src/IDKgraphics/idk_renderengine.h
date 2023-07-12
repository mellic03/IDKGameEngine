#pragma once

#include <unordered_map>
#include "IDKcommon/IDKcommon.h"

#include "idk_model_manager.h"
#include "IDKglInterface/IDKglInterface.h"
#include "idk_drawmethods.h"

#include "idk_camera.h"
#include "IDKlightsource/IDKlightsource.h"
#include "primitives/primitives.h"

#define IDK_MAX_POINTLIGHTS 10
#define IDK_MAX_SPOTLIGHTS 10
#define IDK_MAX_DIRLIGHTS 3

namespace idk { class RenderEngine; };

#define modelqueue_t std::unordered_map<GLuint, idk::vector<idk::pair<int, idk::Transform>>>

class idk::RenderEngine
{
private:
    int                                 _res_x, _res_y;

    SDL_Window *                        _SDL_window;
    SDL_GLContext                       _SDL_gl_context;

    // idk::glFramebuffers ------------------------------------
    /***/
    glFramebuffer                       _gb_geometry_buffer;
    glFramebuffer                       _screenquad_buffer;
    glFramebuffer                       _colorgrade_buffer;
    // --------------------------------------------------------

    // Shaders ------------------------------------------------
    /***/
    GLuint                              _dirshadow_shader;
    GLuint                              _gb_geometry_shader;
    GLuint                              _screenquad_shader;
    GLuint                              _colorgrade_shader;
    GLuint                              _fxaa_shader;
    // --------------------------------------------------------

    // UBO ----------------------------------------------------
    glUBO                               _UBO_camera;
    glUBO                               _UBO_pointlights;
    glUBO                               _UBO_spotlights;
    glUBO                               _UBO_dirlights;

    glFramebuffer                       _dirlight_depthmap_buffer;
    // --------------------------------------------------------

    GLuint                              _quad_VAO, _quad_VBO;

    int                                 _active_camera_id;
    Allocator<Camera>                   _camera_allocator;
    ModelManager                        _model_manager;
    Allocator<lightsource::Point>       _pointlight_allocator;
    Allocator<lightsource::Spot>        _spotlight_allocator;

    Allocator<lightsource::Dir>         _dirlight_allocator;
    Allocator<GLuint>                   _dirlight_shadowmap_allocator;
    Allocator<glm::mat4>                _dirlight_lightspacematrix_allocator;


    modelqueue_t        _model_draw_queue;
    modelqueue_t        _untextured_model_queue;

    // Initialization ---------------------------------------------------------------------
    /***/
    void                _init_SDL_OpenGL( std::string windowname, size_t w, size_t h );
    void                _init_screenquad();
    // ------------------------------------------------------------------------------------

    // Draw-related methods ---------------------------------------------------------------
    /***/
    void                _update_UBO_camera();
    void                _update_UBO_pointlights();
    void                _update_UBO_spotlights();
    void                _update_UBO_dirlights();
    
    void                _shadowpass_pointlights();
    void                _shadowpass_spotlights();
    void                _shadowpass_dirlights();

    void                _render_screenquad( GLuint shader, glFramebuffer &in, glFramebuffer &out );
    void                _render_screenquad( GLuint shader, glFramebuffer &in );

    // void                _bind_material( idk::Material & );
    // void                _draw_model( idk::Model &, idk::Transform & );
    // void                _draw_basic( idk::Model &, idk::Transform & );
    // ------------------------------------------------------------------------------------


public:

    // Built-in shaders -------------------------------------------------------------------
    /***/
    GLuint                              solid_shader;
    // ------------------------------------------------------------------------------------

    // Built-in primitives ----------------------------------------------------------------
    /***/
    static GLuint                       SPHERE_PRIMITIVE;
    static GLuint                       CUBE_PRIMITIVE;
    // ------------------------------------------------------------------------------------

                                        RenderEngine( std::string windowname, size_t w, size_t h );

    int                                 createCamera();
    idk::Camera &                       getCamera()         { return _camera_allocator.get(_active_camera_id); };

    int                                 createPointlight();
    int                                 createSpotlight();
    int                                 createDirlight();

    Allocator<lightsource::Point> &     pointlights()   { return _pointlight_allocator; };
    Allocator<lightsource::Spot> &      spotlights()    { return _spotlight_allocator; };
    Allocator<lightsource::Dir> &       dirlights()     { return _dirlight_allocator; };

    ModelManager &                      modelManager()  { return _model_manager; };
    void                                drawModel( GLuint shader_id, int model_id, Transform &transform );
    void                                drawUntextured( GLuint shader_id, int model_id, Transform &transform );

    void                                beginFrame();
    void                                endFrame();
    void                                resize( int w, int h );

    int                                 width()  const  { return _res_x; };
    int                                 height() const  { return _res_y; };

};

#undef modelqueue_t

