#pragma once

#include <unordered_map>
#include <queue>

#include "libidk/libidk.h"

#include "idk_model_manager.h"
#include "libidk/IDKgl.hpp"
#include "idk_drawmethods.h"

#include "idk_camera.h"
#include "IDKlighting/IDKlighting.hpp"
#include "primitives/primitives.h"
#include "idk_noisegen.hpp"
#include "IDKlighting/IDKlighting.hpp"

#define IDK_MAX_POINTLIGHTS 10
#define IDK_MAX_SPOTLIGHTS 10
#define IDK_MAX_DIRLIGHTS 10


namespace idk { class RenderEngine; };

#define modelqueue_t   std::unordered_map<GLuint, std::vector<idk::pair<int, glm::mat4>>>

class idk::RenderEngine
{
public:

    enum InitFlag
    {
        NONE              = 0,
        INIT_PROGRAMS     = 1 << 0,
        INIT_FRAMEBUFFERS = 1 << 1,
        INIT_LIGHTING     = 1 << 2,
        INIT_MODELS       = 1 << 3,
        INIT_CAMERA       = 1 << 4,
        INIT_NOISE        = 1 << 5,
        INIT_HEADLESS     = 1 << 6
    };

private:
    glm::ivec2                          m_resolution;

    SDL_Window *                        m_SDL_window;
    SDL_GLContext                       m_SDL_gl_context;

    // idk::glFramebuffers ------------------------------------
    /***/
    static const size_t                 NUM_SCRATCH_BUFFERS     = 8;
    static const size_t                 ATTACHMENTS_PER_BUFFER  = 1;

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
    std::vector<std::string>            m_shader_names;
    // --------------------------------------------------------

    // UBO ----------------------------------------------------
    /***/
    glUBO                               m_UBO_camera;
    glUBO                               m_UBO_pointlights;
    // glUBO                               m_UBO_spotlights;
    glUBO                               m_UBO_dirlights;
    // --------------------------------------------------------


    int                                 m_active_camera_id;
    Allocator<Camera>                   m_camera_allocator;
    ModelManager                        m_model_manager;
    idk::LightSystem                    m_lightsystem;


    modelqueue_t                        m_model_draw_queue;
    modelqueue_t                        m_wireframe_draw_queue;
    std::vector<pair<int, glm::mat4>>   m_shadowcast_queue;
    std::vector<Pointlight>             m_pointlight_queue;

    // Initialization ---------------------------------------------------------------------
    /***/
    void                                init_SDL_OpenGL( std::string windowname, size_t w, size_t h, InitFlag flags );
    void                                init_screenquad();
    void                                init_framebuffers( int width, int height );
    void                                init_all( std::string name, int w, int h );
    // ------------------------------------------------------------------------------------

    // Draw-related methods ---------------------------------------------------------------
    /***/
    void                                update_UBO_camera();
    void                                update_UBO_dirlights();
    void                                update_UBO_pointlights();
    void                                shadowpass_dirlights();


    /** Run a shader on the output textures of "in" and render the result to the default frame buffer */
    static void    f_fbfb  ( glShader &, glFramebuffer &in );
    static void    tex2tex ( glShader &, glFramebuffer &a,  glFramebuffer &b, glFramebuffer &out );
    // ------------------------------------------------------------------------------------


public:

    static void    tex2tex ( glShader &, glFramebuffer &in, glFramebuffer &out );

    GLuint                              m_quad_VAO, m_quad_VBO;
    GLuint                              solid_shader;

    std::vector<GLuint> skyboxes;
    std::vector<std::pair<GLuint, GLuint>> skyboxes_IBL;
    int current_skybox;
    GLuint BRDF_LUT;



    // Built-in primitives ----------------------------------------------------------------
    /***/
    static GLuint                       SPHERE_PRIMITIVE;
    static GLuint                       CUBE_PRIMITIVE;
    static GLuint                       CRATE_PRIMITIVE;
    // ------------------------------------------------------------------------------------
    void                                compileShaders();

    void                                init( std::string name, int w, int h, InitFlag flags=InitFlag::NONE );

    SDL_Window *                        SDLWindow()     { return m_SDL_window;      };
    SDL_GLContext                       SDLGLContext()  { return m_SDL_gl_context;  };

    int                                 createCamera();
    void                                useCamera( int cam_id ) { m_active_camera_id = cam_id; };
    idk::Camera &                       getCamera( int cam_id ) { return m_camera_allocator.get(cam_id); };
    idk::Camera &                       getCamera(            ) { return getCamera(m_active_camera_id);  };


    idk::LightSystem &                  lightSystem()   { return m_lightsystem; };
    ModelManager &                      modelManager()  { return m_model_manager; };

    int                                 loadSkybox( const std::string &filepath );

    void                                drawModel ( GLuint      shader_id,
                                                    int         model_id,
                                                    glm::mat4 & model_mat );

    void                                drawModel_now( glShader &   program,
                                                       int          model_id,
                                                       glm::mat4 &  model_mat );

    void                                drawShadowCaster( int model_id, glm::mat4 & );

    void                                drawPointlight( Pointlight light );


    GLuint                              createProgram( std::string name, std::string, std::string, std::string );
    glShader &                          getProgram( const std::string &name ) { return m_shaders[name]; };
    std::vector<std::string>            programNames() { return m_shader_names; };

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

