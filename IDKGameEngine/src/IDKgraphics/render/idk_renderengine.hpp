#pragma once

#include "idk_sdl_glew_init.hpp"

#include <unordered_map>

#include <libidk/IDKgl.hpp>
#include <libidk/IDKcontainers/idk_allocator.hpp>

#include "idk_renderqueue.hpp"

#include "../camera/idk_camera.hpp"
#include "../idk_noisegen.hpp"
#include "../lighting/IDKlighting.hpp"

#define IDK_MAX_POINTLIGHTS 10
#define IDK_MAX_SPOTLIGHTS 10
#define IDK_MAX_DIRLIGHTS 10


namespace idk { class RenderEngine; };


class idk::RenderEngine
{
private:

    internal::SDL_GLEW_Initializer      m_initializer;
    glm::ivec2                          m_resolution;

    // idk::glFramebuffers ------------------------------------
    /***/
    static const size_t                 NUM_SCRATCH_BUFFERS    = 8;
    static const size_t                 ATTACHMENTS_PER_BUFFER = 1;

    std::vector<glFramebuffer>          m_scratchbufs0;
    std::vector<glFramebuffer>          m_scratchbufs1;
    std::vector<glFramebuffer>          m_scratchbufs2;
    std::vector<glFramebuffer>          m_scratchbufs3;

    glFramebuffer                       m_mainbuffer_0;
    glFramebuffer                       m_mainbuffer_1;

    glFramebuffer                       m_deferred_geom_buffer;
    glFramebuffer                       m_volumetrics_buffer;
    // -----------------------------------------------------------------------------------------

    // Shaders
    // -----------------------------------------------------------------------------------------
    std::map<std::string, glShader>     m_shaders;
    std::vector<std::string>            m_shader_names;
    // -----------------------------------------------------------------------------------------

    // UBO
    // -----------------------------------------------------------------------------------------
    glUBO                               m_UBO_pointlights;
    glUBO                               m_UBO_dirlights;
    glUBO                               m_UBO_camera;
    glUBO                               m_UBO_armature;
    // -----------------------------------------------------------------------------------------


    int                                 m_active_camera_id;
    Allocator<Camera>                   m_camera_allocator;
    idk::ModelSystem                    m_modelsystem;
    idk::LightSystem                    m_lightsystem;

    // Render queues
    // -----------------------------------------------------------------------------------------
    idk::Allocator<idk::RenderQueue>    m_private_RQs;
    idk::Allocator<idk::RenderQueue>    m_public_RQs;

    int                                 m_terrain_RQ;

    idk::RenderQueue                    m_render_queue;
    idk::RenderQueue                    m_anim_render_queue;
    idk::RenderQueue                    m_shadow_render_queue;
    idk::RenderQueue                    m_shadow_anim_render_queue;
    // -----------------------------------------------------------------------------------------

    // Initialization
    // -----------------------------------------------------------------------------------------
    /***/
    void                                init_SDL_OpenGL( std::string windowname, size_t w, size_t h, uint32_t flags );
    void                                init_screenquad();
    void                                init_framebuffers( int width, int height );
    void                                init_all( std::string name, int w, int h );
    // -----------------------------------------------------------------------------------------

    // Draw-related methods
    // -----------------------------------------------------------------------------------------
    void                                update_UBO_camera();
    void                                update_UBO_dirlights();
    void                                update_UBO_pointlights();
    void                                shadowpass_dirlights();
    // -----------------------------------------------------------------------------------------


    idk::RenderQueue &                  _getRenderQueue( int id );

    int                                 _createRenderQueue( const std::string &program_name,
                                                            const idk_drawmethod & );


    // Render stages    
    // ------------------------------------------------------------------------------------
    void RenderStage_deferred_geometry( idk::Camera &, float dtime );
    void RenderStage_deferred_lighting( idk::Camera &, float dtime );

    void PostProcess_bloom();
    void PostProcess_chromatic_aberration( glFramebuffer *buffer_A, glFramebuffer *buffer_B );
    void PostProcess_colorgrading( idk::Camera &, glFramebuffer *A, glFramebuffer *B );

    void RenderStage_postprocessing( idk::Camera & );
    // ------------------------------------------------------------------------------------



    /** Run a shader on the output textures of "in" and render the result to the default frame buffer */
    static void    f_fbfb  ( glShader &, glFramebuffer &in );
    static void    tex2tex ( glShader &, glFramebuffer &a,  glFramebuffer &b, glFramebuffer &out );
    // ------------------------------------------------------------------------------------


public:
    static void    tex2tex ( glShader &, glFramebuffer &in, glFramebuffer &out );

    const static uint32_t               ARMATURE_MAX_BONES = 70;

    GLuint                              m_quad_VAO, m_quad_VBO;
    GLuint                              solid_shader;

    std::vector<GLuint>                     skyboxes;
    std::vector<std::pair<GLuint, GLuint>>  skyboxes_IBL;
    int                                     current_skybox = 0;
    GLuint                                  BRDF_LUT;


                                        RenderEngine( const std::string &name, int w, int h,
                                                      uint8_t gl_version, uint32_t flags=0 );

    void                                compileShaders();

    SDL_Window *                        SDLWindow()    { return m_initializer.SDL_window;     };
    SDL_GLContext                       SDLGLContext() { return m_initializer.SDL_GL_context; };

    int                                 createCamera();
    void                                useCamera( int cam_id ) { m_active_camera_id = cam_id; };
    idk::Camera &                       getCamera( int cam_id ) { return m_camera_allocator.get(cam_id); };
    idk::Camera &                       getCamera(            ) { return getCamera(m_active_camera_id);  };
    idk::Allocator<Camera> &            getCameras() { return m_camera_allocator; };


    idk::LightSystem &                  lightSystem() { return m_lightsystem; };
    ModelSystem &                       modelSystem() { return m_modelsystem; };


    int                                 loadSkybox( const std::string &filepath );

    int                                 createRenderQueue( const std::string &program_name,
                                                           const RenderQueueConfig &,
                                                           const idk_drawmethod & );

    idk::RenderQueue &                  getRenderQueue( int id );

    void                                drawModelRQ( int rq, int model, const glm::mat4 & );
    void                                drawModelRQ( int rq, int model, int animator, const glm::mat4 & );

    void                                drawModel( int model, int animator, const glm::mat4 & );
    void                                drawModel( int model, const glm::mat4 & );

    void                                drawShadowCaster( int model, int animator, const glm::mat4 & );
    void                                drawShadowCaster( int model, const glm::mat4 & );

    GLuint                              createProgram( const std::string &name,
                                                       const std::string &root,
                                                       const std::string &vs,
                                                       const std::string &fs );

    glShader &                          getProgram ( const std::string &name ) { return m_shaders[name]; };
    std::map<std::string, glShader> &   getPrograms() { return m_shaders; };

    void                                beginFrame();
    void                                endFrame( float dt );
    void                                swapWindow();
    void                                resize( int w, int h );
    // void                                blitFramebuffer( const idk::glFramebuffer &fb );

    glm::ivec2                          resolution() const { return m_resolution;   };
    int                                 width()      const { return m_resolution.x; };
    int                                 height()     const { return m_resolution.y; };

};


