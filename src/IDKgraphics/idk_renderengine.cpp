#include "idk_renderengine.h"

#include <filesystem>


GLuint worley_texture;


float dist_to_closest(glm::vec3 point, std::vector<glm::vec3> points)
{
    float min_dist = INFINITY;

    for (glm::vec3 p: points)
    {
        float dist = glm::distance(point, p);

        if (dist < min_dist)
            min_dist = dist;
    }

    return min_dist;
}


GLuint noisegen_3D_Worley()
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);


    constexpr int num_points = 1000;
    constexpr int width = 50;

    std::vector<glm::vec3> points(num_points);
    for (int i=0; i<num_points; i++)
    {
        float x = (float)(rand() % (width*1000)) / 1000.0f;
        float y = (float)(rand() % (width*1000)) / 1000.0f;
        float z = (float)(rand() % (width*1000)) / 1000.0f;

        points[i] = glm::vec3(x, y, z);
    }

    float *texture_data = new float[width*width*width];

    float max_value = 0.0f;

    for (int z=0; z<width; z++)
    {
        for (int y=0; y<width; y++)
        {
            for (int x=0; x<width; x++)
            {
                float dist = dist_to_closest(glm::vec3(x, y, z), points);
                if (dist > max_value)
                    max_value = dist;
                texture_data[z*width*width + y*width + x] = dist;
            }
        }
    }


    for (int z=0; z<width; z++)
    {
        for (int y=0; y<width; y++)
        {
            for (int x=0; x<width; x++)
            {
                texture_data[z*width*width + y*width + x] /= max_value;
                texture_data[z*width*width + y*width + x] = texture_data[z*width*width + y*width + x];
            }
        }
    }


    glTexImage3D( GL_TEXTURE_3D, 0, GL_R32F, width, width, width,
                         0, GL_RED, GL_FLOAT, texture_data );

    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, 0);

    delete[] texture_data;

    return texture;
}


// Static members -----------------------------
GLuint  idk::RenderEngine::SPHERE_PRIMITIVE;
GLuint  idk::RenderEngine::CUBE_PRIMITIVE;
// --------------------------------------------

void
idk::RenderEngine::f_init_SDL_OpenGL( std::string windowname, size_t w, size_t h)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error creating window\n";
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    m_SDL_window = SDL_CreateWindow(
        windowname.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);


    m_SDL_gl_context = SDL_GL_CreateContext(m_SDL_window);
    SDL_GL_MakeCurrent(m_SDL_window, m_SDL_gl_context);
    SDL_GL_SetSwapInterval(0); // vsync
    SDL_SetRelativeMouseMode(SDL_FALSE);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error initializing glew\n";
        exit(1);
    }

    gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
}


void
idk::RenderEngine::f_init_screenquad()
{
    float quad_vertices[] = {
      -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
      -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,
       1.0f, -1.0f,  0.0f,  1.0f,  0.0f,

      -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
       1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
       1.0f,  1.0f,  0.0f,  1.0f,  1.0f
    };

    // Send screen quad to GPU
    // ------------------------------------------------------------------------------------
    gl::genVertexArrays(1, &m_quad_VAO);
    gl::genBuffers(1, &m_quad_VBO);

    gl::bindVertexArray(m_quad_VAO);
    gl::bindBuffer(GL_ARRAY_BUFFER, m_quad_VBO);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    gl::vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    gl::enableVertexAttribArray(0);

    gl::vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 3*sizeof(float));
    gl::enableVertexAttribArray(1);
    // ------------------------------------------------------------------------------------
}


void
idk::RenderEngine::compileShaders()
{
    std::cout << "compiling shaders\n";

    m_background_shader = glInterface::compileProgram(
        "shaders/deferred/", "background.vs", "background.fs"
    );
    m_lighting_background_shader = glInterface::compileProgram(
        "shaders/deferred/", "lightingpass.vs", "lightingpass_background.fs"
    );
    m_deferred_geometrypass_shader = glInterface::compileProgram(
        "shaders/deferred/", "geometrypass.vs", "geometrypass.fs"
    );
    m_deferred_lightingpass_shader = glInterface::compileProgram(
        "shaders/deferred/", "lightingpass.vs", "lightingpass.fs"
    );
    m_dirlight_vol_shader = glInterface::compileProgram(
        "shaders/deferred/", "screenquad.vs", "volumetric_dirlight.fs"
    );

    m_guassian_blur_shader = glInterface::compileProgram(
        "shaders/", "screenquad.vs", "postprocess/guassianblur.fs"
    );

    m_additive_shader   = glInterface::compileProgram("shaders/", "screenquad.vs", "postprocess/additive.fs");
    m_colorgrade_shader = glInterface::compileProgram("shaders/", "screenquad.vs", "postprocess/colorgrade.fs");
    m_fxaa_shader       = glInterface::compileProgram("shaders/", "screenquad.vs", "postprocess/fxaa.fs");
    m_dirshadow_shader  = glInterface::compileProgram("shaders/", "dirshadow.vs", "dirshadow.fs");
    solid_shader        = glInterface::compileProgram("shaders/", "vsin_pos_only.vs", "solid.fs");
}


void
idk::RenderEngine::f_gen_idk_framebuffers( int w, int h )
{
    m_deferred_geom_buffer      = glInterface::genIdkFramebuffer(    w,     h,   4);
    m_volumetrics_buffer        = glInterface::genIdkFramebuffer(  w/2,   h/2,   1);
    m_colorgrade_buffer         = glInterface::genIdkFramebuffer(    w,     h,   1);
    m_fxaa_buffer               = glInterface::genIdkFramebuffer(    w,     h,   1);
    m_blurred_buffer            = glInterface::genIdkFramebuffer(    w,     h,   1);
    m_blurred_buffer2           = glInterface::genIdkFramebuffer(    w,     h,   1);
    m_final_buffer              = glInterface::genIdkFramebuffer(    w,     h,   1);
    m_dirlight_depthmap_buffer  = glInterface::genIdkFramebuffer( 4096,  4096,   4);

}



idk::RenderEngine::RenderEngine( std::string name, int w, int h, int res_divisor )
{
    glInterface::init();

    m_resolution = glm::ivec2(w, h);
    f_init_SDL_OpenGL(name, w, h);
    f_init_screenquad();

    RenderEngine::CUBE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::cube, "");
    RenderEngine::SPHERE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::icosphere, "");
   
    f_gen_idk_framebuffers(w, h);
    compileShaders();

    m_active_camera_id = createCamera();

    m_UBO_camera      = glUBO(2, 2*sizeof(glm::mat4) + sizeof(glm::vec4));
    m_UBO_pointlights = glUBO(3, 16 + IDK_MAX_POINTLIGHTS*sizeof(Pointlight));
    m_UBO_spotlights  = glUBO(4, 16 + IDK_MAX_SPOTLIGHTS*sizeof(Spotlight));
    m_UBO_dirlights   = glUBO(5, 16 + IDK_MAX_DIRLIGHTS*sizeof(Dirlight) + IDK_MAX_DIRLIGHTS*sizeof(glm::mat4));

    worley_texture = noisegen_3D_Worley();
}



void
idk::RenderEngine::f_fbfb( GLuint shader, glFramebuffer &in, glFramebuffer &out )
{
    static float inc0 = 0.0f;
    static float inc1 = 0.0f;
    static float inc2 = 0.0f;


    glInterface::bindIdkFramebuffer(out);
    glInterface::useProgram(shader);

    glInterface::setUniform_texture("un_dirlight_depthmaps[0]", m_dirlight_shadowmap_allocator.get(0));
    glInterface::setUniform_texture3D("un_worley", worley_texture);
    glInterface::setUniform_float("un_increment_0", inc0);
    glInterface::setUniform_float("un_increment_1", inc1);
    glInterface::setUniform_float("un_increment_2", inc2);


    inc0 += 0.0001f;
    inc1 += 0.000015f;
    inc2 += 0.00003f;


    for (size_t i=0; i < in.output_textures.size(); i++)
    {
        std::string name = "un_texture_" + std::to_string(i);
        glInterface::setUniform_texture(name, in.output_textures[i]);
    }

    gl::bindVertexArray(m_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


void
idk::RenderEngine::f_fbfb( GLuint shader, glFramebuffer &in )
{
    glInterface::unbindIdkFramebuffer(m_resolution.x, m_resolution.y);
    glInterface::useProgram(shader);

    for (size_t i=0; i < in.output_textures.size(); i++)
        glInterface::setUniform_texture("un_texture_" + std::to_string(i), in.output_textures[0]);

    gl::bindVertexArray(m_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


void
idk::RenderEngine::f_fbfb( GLuint shader, GLuint tex0, GLuint tex1 )
{
    glInterface::unbindIdkFramebuffer(m_resolution.x, m_resolution.y);
    glInterface::useProgram(shader);

    glInterface::setUniform_texture("un_texture_0", tex0);
    glInterface::setUniform_texture("un_texture_1", tex1);

    gl::bindVertexArray(m_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


void
idk::RenderEngine::f_fbfb( GLuint shader, GLuint tex0, GLuint tex1, glFramebuffer &out )
{
    glInterface::bindIdkFramebuffer(out);
    glInterface::useProgram(shader);

    glInterface::setUniform_texture("un_dirlight_depthmaps[0]", m_dirlight_shadowmap_allocator.get(0));

    glInterface::setUniform_texture("un_texture_0", tex0);
    glInterface::setUniform_texture("un_texture_1", tex1);

    gl::bindVertexArray(m_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


int
idk::RenderEngine::createCamera()
{
    int camera_id = m_camera_allocator.add();
    m_camera_allocator.get(camera_id).aspect(m_resolution.x, m_resolution.y);
    return camera_id;
}


int
idk::RenderEngine::createPointlight()
{
    return m_pointlight_allocator.add();
}


int
idk::RenderEngine::createSpotlight()
{
    return m_spotlight_allocator.add();
}


int
idk::RenderEngine::createDirlight()
{
    m_dirlight_shadowmap_allocator.add();
    m_dirlight_lightspacematrix_allocator.add();
    return m_dirlight_allocator.add();
}


void
idk::RenderEngine::drawModel( GLuint shader_id, int model_id, Transform &transform )
{
    m_model_draw_queue[shader_id].push({model_id, transform});
}

void
idk::RenderEngine::drawUntextured( GLuint shader_id, int model_id, Transform &transform )
{
    // m_untextured_model_queue[shader_id].push({model_id, transform});
}


void
idk::RenderEngine::f_shadowpass_pointlights()
{

}


void
idk::RenderEngine::f_shadowpass_spotlights()
{

}


void
idk::RenderEngine::f_shadowpass_dirlights()
{
    glInterface::clearIdkFramebuffer(m_dirlight_depthmap_buffer);
    glInterface::bindIdkFramebuffer(m_dirlight_depthmap_buffer);
    glInterface::useProgram(m_dirshadow_shader);

    float near_plane = 1.0f, far_plane = 50.0f;

    RenderEngine &ren = *this;
    auto &queue = m_model_draw_queue;
    auto &depthmaps = m_dirlight_shadowmap_allocator;
    auto &framebuffer = m_dirlight_depthmap_buffer;

    int i = 0;
    dirlights().for_each(
    [&i, &ren, &queue, &depthmaps, &framebuffer](int light_id, Dirlight &light)
    {
        glm::mat4 projection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 0.1f, 150.0f);
        glm::mat4 view = glm::lookAt(
            -10.0f*glm::vec3(light.direction) + ren.getCamera().transform().position(),
            glm::vec3(0.0f) + ren.getCamera().transform().position(),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        glm::mat4 lightspacematrix = projection * view;
        glInterface::setUniform_mat4("un_lightspacematrix", lightspacematrix);

        for (auto &[shader_id, vec]: queue)
        {
            for (auto &[model_id, transform]: vec)
            {
                drawmethods::draw_untextured(
                    ren.modelManager().getModel(model_id),
                    transform,
                    ren.modelManager().getMaterials()
                );
            }
        }

        depthmaps.get(light_id) = framebuffer.output_textures[0];
        i += 1;
    });
}


void
idk::RenderEngine::f_update_UBO_camera()
{
    idk::Camera &camera = getCamera();
    m_UBO_camera.bind();
    m_UBO_camera.add<glm::mat4>(glm::value_ptr(camera.view()));
    m_UBO_camera.add<glm::mat4>(glm::value_ptr(camera.projection()));
    m_UBO_camera.add<glm::vec3>(glm::value_ptr(camera.transform().position()));
    m_UBO_camera.unbind();
}


void
idk::RenderEngine::f_update_UBO_pointlights()
{
    int num_pointlights = pointlights().size();

    std::vector<idk::Pointlight> lights;
    pointlights().for_each(
        [&lights](Pointlight &light)
        {
            lights.push_back(light);
        }
    );
    lights.resize(IDK_MAX_POINTLIGHTS);

    m_UBO_pointlights.bind();
    m_UBO_pointlights.add<int>(&num_pointlights);
    m_UBO_pointlights.add(sizeof(Pointlight)*IDK_MAX_SPOTLIGHTS, &(lights[0]));
    m_UBO_pointlights.unbind();
}


void
idk::RenderEngine::f_update_UBO_spotlights()
{
    int num_spotlights = spotlights().size();

    idk::vector<idk::Spotlight> lights;
    spotlights().for_each(
        [&lights](Spotlight &light)
        {
            lights.push(light);
        }
    );
    lights.resize(IDK_MAX_SPOTLIGHTS);

    m_UBO_spotlights.bind();
    m_UBO_spotlights.add<int>(&num_spotlights);
    m_UBO_spotlights.add(sizeof(Spotlight)*IDK_MAX_SPOTLIGHTS, lights.data());
    m_UBO_spotlights.unbind();
}


void
idk::RenderEngine::f_update_UBO_dirlights()
{
    int num_dirlights = dirlights().size();

    idk::vector<idk::Dirlight> lights;
    idk::vector<glm::mat4> matrices;

    idk::Camera &camera = getCamera();

    dirlights().for_each(
        [&lights, &matrices, &camera](Dirlight &light)
        {
            lights.push(light);

            glm::mat4 projection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 0.1f, 150.0f);
            glm::mat4 view = glm::lookAt(
                -10.0f*glm::vec3(light.direction) + camera.transform().position(),
                glm::vec3(0.0f) + camera.transform().position(),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            glm::mat4 lightspacematrix = projection * view;
            matrices.push(lightspacematrix);
        }
    );


    lights.resize(IDK_MAX_DIRLIGHTS);
    matrices.resize(IDK_MAX_DIRLIGHTS);

    m_UBO_dirlights.bind();
    m_UBO_dirlights.add<int>(&num_dirlights);
    m_UBO_dirlights.add(sizeof(Dirlight)*IDK_MAX_DIRLIGHTS, lights.data());
    m_UBO_dirlights.add(sizeof(glm::mat4)*IDK_MAX_DIRLIGHTS, matrices.data());
    m_UBO_dirlights.unbind();
}


void
idk::RenderEngine::beginFrame()
{
    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);
    gl::viewport(0, 0, m_resolution.x, m_resolution.y);
    gl::clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void
idk::RenderEngine::endFrame()
{
    f_shadowpass_pointlights();
    f_shadowpass_spotlights();
    f_shadowpass_dirlights();

    f_update_UBO_camera();
    f_update_UBO_pointlights();
    f_update_UBO_spotlights();
    f_update_UBO_dirlights();

    idk::Camera &camera = getCamera();

    // Deferred geometry pass --------------------------------------------
    glInterface::bindIdkFramebuffer(m_deferred_geom_buffer);

    for (auto &[shader_id, vec]: m_model_draw_queue)
    {
        glInterface::useProgram(shader_id);

        for (auto &[model_id, transform]: vec)
        {
            drawmethods::draw_textured(
                modelManager().getModel(model_id),
                transform,
                modelManager().getMaterials()
            );
            glInterface::freeTextureUnitIDs();
        }
        vec.clear();
    }
    m_model_draw_queue.clear();
    gl::bindVertexArray(0);
    // -------------------------------------------------------------------

    // Deferred lighting pass --------------------------------------------
    glInterface::useProgram(m_background_shader);
    glm::mat4 modelmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.9f * camera.farPlane()));
    modelmat = glm::scale(modelmat, glm::vec3(300.0f, 300.0f, 1.0f));
    glInterface::setUniform_mat4("un_model", modelmat);

    gl::bindVertexArray(m_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);
    gl::bindVertexArray(0);

    gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);

    // Blinn-Phong lighting
    f_fbfb(
        m_deferred_lightingpass_shader,
        m_deferred_geom_buffer,
        m_final_buffer
    );

    // Volumetric directional lights
    f_fbfb(
        m_dirlight_vol_shader,
        m_deferred_geom_buffer,
        m_volumetrics_buffer
    );

    // f_fbfb(
    //     m_guassian_blur_shader,
    //     m_volumetrics_buffer,
    //     m_blurred_buffer
    // );

    f_fbfb(
        m_fxaa_shader,
        m_volumetrics_buffer,
        m_blurred_buffer
    );

    // Combine Blinn-Phong and volumetrics    
    f_fbfb(
        m_additive_shader,
        m_final_buffer.output_textures[0],
        m_blurred_buffer.output_textures[0],
        m_colorgrade_buffer
    );

    // Post processing
    f_fbfb(m_colorgrade_shader, m_colorgrade_buffer, m_fxaa_buffer);
    f_fbfb(m_fxaa_shader, m_fxaa_buffer);

    gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
    // -------------------------------------------------------------------

    glInterface::clearIdkFramebuffers(
        m_deferred_geom_buffer,
        m_volumetrics_buffer,
        m_fxaa_buffer,
        m_colorgrade_buffer,
        m_final_buffer
    );

    glInterface::unbindIdkFramebuffer(m_resolution.x, m_resolution.y);
}


void
idk::RenderEngine::swapWindow()
{
    SDL_GL_SwapWindow(m_SDL_window);
}


void
idk::RenderEngine::resize( int w, int h )
{
    m_resolution = { w, h };
    f_gen_idk_framebuffers(w, h);
    getCamera().aspect(w, h);
}

