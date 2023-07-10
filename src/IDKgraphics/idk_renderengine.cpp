#include "idk_renderengine.h"

#include <filesystem>


// Static methods -------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------




// Static members -----------------------------
GLuint  idk::RenderEngine::SPHERE_PRIMITIVE;
GLuint  idk::RenderEngine::CUBE_PRIMITIVE;
// --------------------------------------------

void
idk::RenderEngine::_init_SDL_OpenGL( std::string windowname, size_t w, size_t h)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error creating window\n";
        exit(1);
    }

    _SDL_window = SDL_CreateWindow(
        windowname.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

    _SDL_gl_context = SDL_GL_CreateContext(_SDL_window);
    SDL_GL_MakeCurrent(_SDL_window, _SDL_gl_context);
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
idk::RenderEngine::_init_screenquad()
{
    float quad_vertices[] = {
      -1.0f,  1.0f,  0.0f,  1.0f,
      -1.0f, -1.0f,  0.0f,  0.0f,
       1.0f, -1.0f,  1.0f,  0.0f,

      -1.0f,  1.0f,  0.0f,  1.0f,
       1.0f, -1.0f,  1.0f,  0.0f,
       1.0f,  1.0f,  1.0f,  1.0f
    };

    // Send screen quad to GPU
    // ------------------------------------------------------------------------------------
    gl::genVertexArrays(1, &_quad_VAO);
    gl::genBuffers(1, &_quad_VBO);

    gl::bindVertexArray(_quad_VAO);
    gl::bindBuffer(GL_ARRAY_BUFFER, _quad_VBO);
    gl::bufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    gl::vertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    gl::enableVertexAttribArray(0);

    gl::vertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 2*sizeof(float));
    gl::enableVertexAttribArray(1);
    // ------------------------------------------------------------------------------------
}


idk::RenderEngine::RenderEngine( std::string windowname, size_t w, size_t h ):
_res_x(w), _res_y(h),
_gb_geometry_buffer(4), _screenquad_buffer(1), _colorgrade_buffer(1)
{
    glInterface::init();
    _init_SDL_OpenGL(windowname, _res_x, _res_y);
    _init_screenquad();

    _active_camera_id = createCamera();

    RenderEngine::CUBE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::cube, "");
    RenderEngine::SPHERE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::icosphere, "");

    _gb_geometry_shader = glInterface::compileProgram("assets/shaders/", "gb_geom.vs", "gb_geom.fs");
    _screenquad_shader = glInterface::compileProgram("assets/shaders/", "screenquad.vs", "screenquad.fs");
    _colorgrade_shader = glInterface::compileProgram("assets/shaders/", "screenquad.vs", "colorgrade.fs");
    _fxaa_shader = glInterface::compileProgram("assets/shaders/", "screenquad.vs", "fxaa.fs");
    solid_shader = glInterface::compileProgram("assets/shaders/", "vsin_pos_only.vs", "solid.fs");

    glInterface::genIdkFramebuffer(_res_x, _res_y, _gb_geometry_buffer);
    glInterface::genIdkFramebuffer(_res_x, _res_y, _screenquad_buffer);
    glInterface::genIdkFramebuffer(_res_x, _res_y, _colorgrade_buffer);


    _UBO_camera      = glUBO(2, 2*sizeof(glm::mat4) + sizeof(glm::vec4));
    _UBO_pointlights = glUBO(3, 16 + 10*4*sizeof(glm::vec4));
    _UBO_spotlights  = glUBO(4, 16 + 10*6*sizeof(glm::vec4));
}


void
idk::RenderEngine::_render_screenquad( GLuint shader, glFramebuffer &in, glFramebuffer &out )
{
    glInterface::bindIdkFramebuffer(_res_x, _res_y, out);
    glInterface::useProgram(shader);

    for (int i=0; i < in.textures.size(); i++)
        glInterface::setUniform_texture("un_texture_" + std::to_string(i), in.textures[0]);

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


void
idk::RenderEngine::_render_screenquad( GLuint shader, glFramebuffer &in )
{
    glInterface::unbindIdkFramebuffer(_res_x, _res_y);
    glInterface::useProgram(shader);

    for (int i=0; i < in.textures.size(); i++)
        glInterface::setUniform_texture("un_texture_" + std::to_string(i), in.textures[0]);

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);

    glInterface::freeTextureUnitIDs();
}


int
idk::RenderEngine::createCamera()
{
    int camera_id = _camera_allocator.add();
    _camera_allocator.get(camera_id).aspect(_res_x, _res_y);
    return camera_id;
}


int
idk::RenderEngine::createPointlight()
{
    return _pointlight_allocator.add();
}


int
idk::RenderEngine::createSpotlight()
{
    return _spotlight_allocator.add();
}


void
idk::RenderEngine::drawModel( GLuint shader_id, int model_id, Transform &transform )
{
    _model_draw_queue[shader_id].push({model_id, transform});
}


void
idk::RenderEngine::drawUntextured( GLuint shader_id, int model_id, Transform &transform )
{
    _untextured_model_queue[shader_id].push({model_id, transform});
}


void
idk::RenderEngine::_update_UBO_camera()
{
    idk::Camera &camera = getCamera();
    _UBO_camera.bind();
    _UBO_camera.add<glm::mat4>(glm::value_ptr(camera.view()));
    _UBO_camera.add<glm::mat4>(glm::value_ptr(camera.projection()));
    _UBO_camera.add<glm::vec3>(glm::value_ptr(camera.transform().position()));
    _UBO_camera.unbind();
}


void
idk::RenderEngine::_update_UBO_pointlights()
{
    int num_pointlights = pointlights().size();
    std::vector<glm::vec4> position(IDK_MAX_POINTLIGHTS);
    std::vector<glm::vec4> ambient(IDK_MAX_POINTLIGHTS);
    std::vector<glm::vec4> diffuse(IDK_MAX_POINTLIGHTS);
    std::vector<glm::vec4> attenuation(IDK_MAX_POINTLIGHTS);

    int i = 0;
    pointlights().for_each(
    [&i, &position, &ambient, &diffuse, &attenuation](lightsource::Point &light)
    {
        position[i] = light.position;
        ambient[i] = light.ambient;
        diffuse[i] = light.diffuse;
        attenuation[i] = light.attenuation;
        i += 1;
    });

    size_t sizeof_vec = 4*sizeof(float) * IDK_MAX_POINTLIGHTS;

    _UBO_pointlights.bind();
    _UBO_pointlights.add<int>(&num_pointlights);
    _UBO_pointlights.add(sizeof_vec, &(position[0]));
    _UBO_pointlights.add(sizeof_vec, &(ambient[0]));
    _UBO_pointlights.add(sizeof_vec, &(diffuse[0]));
    _UBO_pointlights.add(sizeof_vec, &(attenuation[0]));
    _UBO_pointlights.unbind();
}


void
idk::RenderEngine::_update_UBO_spotlights()
{
    int num_spotlights = spotlights().size();
    std::vector<glm::vec4> position(IDK_MAX_SPOTLIGHTS);
    std::vector<glm::vec4> direction(IDK_MAX_SPOTLIGHTS);
    std::vector<glm::vec4> ambient(IDK_MAX_SPOTLIGHTS);
    std::vector<glm::vec4> diffuse(IDK_MAX_SPOTLIGHTS);
    std::vector<glm::vec4> attenuation(IDK_MAX_SPOTLIGHTS);
    std::vector<glm::vec4> cutoff(IDK_MAX_SPOTLIGHTS);

    int i = 0;
    spotlights().for_each(
    [&i, &position, &direction, &ambient, &diffuse, &attenuation, &cutoff](lightsource::Spot &light)
    {
        position[i] = light.position;
        direction[i] = light.direction;
        ambient[i] = light.ambient;
        diffuse[i] = light.diffuse;
        attenuation[i] = light.attenuation;
        cutoff[i] = glm::vec4(
            glm::radians(light.inner_cutoff),
            glm::radians(light.outer_cutoff),
            0.0f,
            0.0f
        );

        i += 1;
    });

    size_t sizeof_vec = 4*sizeof(float) * IDK_MAX_SPOTLIGHTS;

    _UBO_spotlights.bind();
    _UBO_spotlights.add<int>(&num_spotlights);
    _UBO_spotlights.add(sizeof_vec, &(position[0]));
    _UBO_spotlights.add(sizeof_vec, &(direction[0]));
    _UBO_spotlights.add(sizeof_vec, &(ambient[0]));
    _UBO_spotlights.add(sizeof_vec, &(diffuse[0]));
    _UBO_spotlights.add(sizeof_vec, &(attenuation[0]));
    _UBO_spotlights.add(sizeof_vec, &(cutoff[0]));
    _UBO_spotlights.unbind();
}


void
idk::RenderEngine::beginFrame()
{
    SDL_GL_SwapWindow(_SDL_window);

    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);
    gl::clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GLCALL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); )
}


void
idk::RenderEngine::endFrame()
{
    glInterface::bindIdkFramebuffer(_res_x, _res_y, _gb_geometry_buffer);
    idk::Camera &camera = getCamera();

    _update_UBO_camera();
    _update_UBO_pointlights();
    _update_UBO_spotlights();

    for (auto &[shader_id, vec]: _model_draw_queue)
    {
        glInterface::useProgram(shader_id);
        for (auto &[model_id, transform]: vec)
        {
            drawmethods::draw_textured(
                modelManager().getModel(model_id),
                transform,
                modelManager().getMaterials()
            );
        }
        vec.clear();
    }
    _model_draw_queue.clear();

    for (auto &[shader_id, vec]: _untextured_model_queue)
    {
        glInterface::useProgram(shader_id);
        for (auto &[model_id, transform]: vec)
        {
            drawmethods::draw_untextured(
                modelManager().getModel(model_id),
                transform
            );
        }
        vec.clear();
    }
    _untextured_model_queue.clear();


    gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);
    _render_screenquad(_screenquad_shader, _gb_geometry_buffer, _screenquad_buffer);
    _render_screenquad(_colorgrade_shader, _screenquad_buffer, _colorgrade_buffer);
    _render_screenquad(_fxaa_shader, _colorgrade_buffer);
    gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);
}


void
idk::RenderEngine::resize( int w, int h )
{
    _res_x = w;
    _res_y = h;

    glInterface::genIdkFramebuffer(w, h, _gb_geometry_buffer);
    glInterface::genIdkFramebuffer(w, h, _screenquad_buffer);
    glInterface::genIdkFramebuffer(w, h, _colorgrade_buffer);

    getCamera().aspect(w, h);
}

