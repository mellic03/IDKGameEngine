#pragma once

#include <unordered_map>
#include <queue>

#include "IDKcommon/IDKcommon.h"

#include "idk_model_manager.h"
#include "IDKglInterface/IDKglInterface.hpp"
#include "idk_drawmethods.h"

#include "idk_camera.h"
#include "IDKlightsource/IDKlightsource.h"
#include "primitives/primitives.h"
#include "idk_noisegen.hpp"

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
    static const size_t                 NUM_SCRATCH_BUFFERS     = 4;
    static const size_t                 ATTACHMENTS_PER_BUFFER  = 1;
    static const size_t                 GBUFFER_NUM_ATTACHMETNS = 4;

    std::vector<glFramebuffer>          m_scratchbufs0;
    std::vector<glFramebuffer>          m_scratchbufs1;
    std::vector<glFramebuffer>          m_scratchbufs2;
    std::vector<glFramebuffer>          m_scratchbufs3;

    glFramebuffer                       m_mainbuffer_0;
    glFramebuffer                       m_mainbuffer_1;

    glFramebuffer                       m_depthbuffer;

    glFramebuffer                       m_deferred_geom_buffer;
    glFramebuffer                       m_volumetrics_buffer;
    std::queue<glFramebuffer>           m_blit_queue;
    // --------------------------------------------------------

    // Shaders ------------------------------------------------
    /***/
    std::map<std::string, glShader>     m_shaders;
    // --------------------------------------------------------

    // UBO ----------------------------------------------------
    /***/
    glUBO                               m_UBO_camera;
    glUBO                               m_UBO_pointlights;
    glUBO                               m_UBO_spotlights;
    glUBO                               m_UBO_dirlights;
    // --------------------------------------------------------


    int                                 m_active_camera_id;
    Allocator<Camera>                   m_camera_allocator;
    ModelManager                        m_model_manager;
    Allocator<Pointlight>               m_pointlight_allocator;
    Allocator<Spotlight>                m_spotlight_allocator;

    Allocator<Dirlight>                 m_dirlight_allocator;
    Allocator<glm::mat4>                m_dirlight_lightspacematrix_allocator;

    modelqueue_t                        m_model_draw_queue;
    modelqueue_t                        m_wireframe_draw_queue;

    // Initialization ---------------------------------------------------------------------
    /***/
    void                                f_init_SDL_OpenGL( std::string windowname, size_t w, size_t h );
    void                                f_init_screenquad();
    void                                f_gen_idk_framebuffers( int width, int height );
    // ------------------------------------------------------------------------------------

    // Draw-related methods ---------------------------------------------------------------
    /***/
    void                                f_update_UBO_camera();
    void                                f_update_UBO_pointlights();
    void                                f_update_UBO_spotlights();
    void                                f_update_UBO_dirlights();
    
    void                                f_shadowpass_pointlights();
    void                                f_shadowpass_spotlights();
    void                                f_shadowpass_dirlights();


    /** Run a shader on the output textures of "in" and render the result to the default frame buffer */
    void    f_fbfb  ( glShader &, glFramebuffer &in );
    void    tex2tex ( glShader &, glFramebuffer &in, glFramebuffer &out );
    void    tex2tex ( glShader &, glFramebuffer &a,  glFramebuffer &b, glFramebuffer &out );
    // ------------------------------------------------------------------------------------


public:
    GLuint                              m_quad_VAO, m_quad_VBO;
    Allocator<GLuint>                   m_dirlight_shadowmap_allocator;
    glFramebuffer                       m_dirlight_depthmap_buffer;
    GLuint solid_shader;

    float                               m_bloom_intensity=0.0f, m_gamma=2.2f, m_exposure=1.0f;
    void                                setBloomIntensity(float f) { m_bloom_intensity = f; };


    glm::vec2 m_r_abbr = glm::vec2(0.0f);
    glm::vec2 m_g_abbr = glm::vec2(0.0f);
    glm::vec2 m_b_abbr = glm::vec2(0.0f);
    float m_abbr_str = 0.0f;

    // Built-in shaders -------------------------------------------------------------------
    /***/
    // ------------------------------------------------------------------------------------

    // Built-in primitives ----------------------------------------------------------------
    /***/
    static GLuint                       SPHERE_PRIMITIVE;
    static GLuint                       CUBE_PRIMITIVE;
    static GLuint                       CRATE_PRIMITIVE;
    // ------------------------------------------------------------------------------------
    void                                compileShaders();

                                        RenderEngine( std::string name, int w, int h, int res_divisor );
    SDL_Window *                        SDLWindow()     { return m_SDL_window;      };
    SDL_GLContext                       SDLGLContext()  { return m_SDL_gl_context;  };

    int                                 createCamera();
    idk::Camera &                       getCamera()     { return m_camera_allocator.get(m_active_camera_id); };

    int                                 createPointlight();
    int                                 createSpotlight();
    int                                 createDirlight();

    Allocator<Pointlight> &             pointlights()   { return m_pointlight_allocator; };
    Allocator<Spotlight> &              spotlights()    { return m_spotlight_allocator; };
    Allocator<Dirlight> &               dirlights()     { return m_dirlight_allocator; };

    ModelManager &                      modelManager()  { return m_model_manager; };
    void                                drawModel( GLuint shader_id, int model_id, Transform &transform );
    void                                drawWireframe( GLuint shader_id, int model_id, Transform &transform );

    void                                createProgram( std::string name, std::string, std::string, std::string );
    glShader &                          getProgram( const std::string &name ) { return m_shaders[name]; };

    void                                beginFrame();
    void                                endFrame();
    void                                swapWindow();
    void                                resize( int w, int h );
    void                                blitFramebuffer( const idk::glFramebuffer &fb );

    glm::ivec2                          resolution() const { return m_resolution;   };
    int                                 width()      const { return m_resolution.x; };
    int                                 height()     const { return m_resolution.y; };

};

#undef modelqueue_t

