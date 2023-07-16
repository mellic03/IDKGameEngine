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

#define modelqueue_t   std::unordered_map<GLuint, idk::vector<idk::pair<int, idk::Transform>>>

class idk::RenderEngine
{
private:
    glm::ivec2                          m_resolution;

    SDL_Window *                        m_SDL_window;
    SDL_GLContext                       m_SDL_gl_context;

    // idk::glFramebuffers ------------------------------------
    /***/
    glFramebuffer                       m_deferred_geom_buffer;
    glFramebuffer                       m_volumetrics_buffer;
    glFramebuffer                       m_colorgrade_buffer;
    glFramebuffer                       m_fxaa_buffer;
    glFramebuffer                       m_final_buffer;

    // --------------------------------------------------------

    // Shaders ------------------------------------------------
    /***/
    GLuint                              m_deferred_geometrypass_shader;
    GLuint                              m_deferred_lightingpass_shader;
    GLuint                              m_dirlight_vol_shader;
    GLuint                              m_background_shader;
    GLuint                              m_lighting_background_shader;

    GLuint                              m_guassian_blur_shader;

    GLuint                              m_dirshadow_shader;
    GLuint                              m_screenquad_shader;

    GLuint                              m_colorgrade_shader;
    GLuint                              m_additive_shader;
    GLuint                              m_fxaa_shader;
    // --------------------------------------------------------

    // UBO ----------------------------------------------------
    glUBO                               m_UBO_camera;
    glUBO                               m_UBO_pointlights;
    glUBO                               m_UBO_spotlights;
    glUBO                               m_UBO_dirlights;

    glFramebuffer                       m_dirlight_depthmap_buffer;
    // --------------------------------------------------------

    GLuint                              _quad_VAO, _quad_VBO;

    int                                 m_active_camera_id;
    Allocator<Camera>                   _camera_allocator;
    ModelManager                        _model_manager;
    Allocator<Pointlight>               _pointlight_allocator;
    Allocator<Spotlight>                _spotlight_allocator;

    Allocator<Dirlight>                 _dirlight_allocator;
    Allocator<GLuint>                   _dirlight_shadowmap_allocator;
    Allocator<glm::mat4>                _dirlight_lightspacematrix_allocator;


    modelqueue_t        _model_draw_queue;
    modelqueue_t        _untextured_model_queue;

    // Initialization ---------------------------------------------------------------------
    /***/
    void                f_init_SDL_OpenGL( std::string windowname, size_t w, size_t h );
    void                f_init_screenquad();
    void                f_gen_idk_framebuffers( int width, int height );
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


    /** Run a shader on the output textures of "in" and render the result to "out" */
    void                f_fbfb( GLuint shader, glFramebuffer &in, glFramebuffer &out );
    
    /** Run a shader on the output textures of "in" and render the result to the default frame buffer */
    void                f_fbfb( GLuint shader, glFramebuffer &in );

    void                f_fbfb( GLuint shader, GLuint tex0, GLuint tex1, glFramebuffer &out );
    void                f_fbfb( GLuint shader, GLuint tex0, GLuint tex1 );
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
    void                                f_compile_shaders();

                                        RenderEngine( std::string name, int w, int h, int res_divisor );
    SDL_Window *                        SDLWindow()     { return m_SDL_window;      };
    SDL_GLContext *                     SDLGLContext()  { return &m_SDL_gl_context; };

    int                                 createCamera();
    idk::Camera &                       getCamera()         { return _camera_allocator.get(m_active_camera_id); };

    int                                 createPointlight();
    int                                 createSpotlight();
    int                                 createDirlight();

    Allocator<Pointlight> &             pointlights()   { return _pointlight_allocator; };
    Allocator<Spotlight> &              spotlights()    { return _spotlight_allocator; };
    Allocator<Dirlight> &               dirlights()     { return _dirlight_allocator; };

    ModelManager &                      modelManager()  { return _model_manager; };
    void                                drawModel( GLuint shader_id, int model_id, Transform &transform );
    void                                drawUntextured( GLuint shader_id, int model_id, Transform &transform );

    void                                beginFrame();
    void                                endFrame();
    void                                swapWindow();
    void                                resize( int w, int h );

    glm::ivec2                          resolution() const { return m_resolution;   };
    int                                 width()      const { return m_resolution.x; };
    int                                 height()     const { return m_resolution.y; };

};

#undef modelqueue_t

