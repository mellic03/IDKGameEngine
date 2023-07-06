#include "idk_renderengine.h"

#include <filesystem>


void
idk::RenderEngine::_init_SDL_OpenGL(size_t w, size_t h)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error creating window\n";
        exit(1);
    }

    _SDL_window = SDL_CreateWindow(
        "IDK Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
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


idk::RenderEngine::RenderEngine(size_t w, size_t h):
_screen_width(w), _screen_height(h),
_gb_geometry_buffer(4), _screenquad_buffer(1), _colorgrade_buffer(1)
{
    glInterface::init();
    _init_SDL_OpenGL(_screen_width, _screen_height);
    _init_screenquad();

    _active_camera_id = createCamera();

    CUBE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::cube, "");
    SPHERE_PRIMITIVE = modelManager().loadOBJ(idk::objprimitives::icosphere, "");

    glInterface::genIdkFramebuffer(_screen_width, _screen_height, _gb_geometry_buffer);
    glInterface::genIdkFramebuffer(_screen_width, _screen_height, _screenquad_buffer);
    glInterface::genIdkFramebuffer(_screen_width, _screen_height, _colorgrade_buffer);

    _gb_geometry_shader = glInterface::compileShaderProgram("assets/shaders/", "gb_geom.vs", "gb_geom.fs");
    _screenquad_shader = glInterface::compileShaderProgram("assets/shaders/", "screenquad.vs", "screenquad.fs");
    _colorgrade_shader = glInterface::compileShaderProgram("assets/shaders/", "screenquad.vs", "colorgrade.fs");
    _fxaa_shader = glInterface::compileShaderProgram("assets/shaders/", "screenquad.vs", "fxaa.fs");
    solid_shader = glInterface::compileShaderProgram("assets/shaders/", "vsin_pos_only.vs", "solid.fs");
    wireframe_shader = glInterface::compileShaderProgram("assets/shaders/", "vsin_pos_only.vs", "wireframe.fs");
}


void
idk::RenderEngine::_render_screenquad( GLuint shader, glFramebuffer &in, glFramebuffer &out )
{
    glInterface::bindIdkFramebuffer(_screen_width, _screen_height, out);
    glInterface::useProgram(shader);

    for (int i=0; i < in.textures.size(); i++)
        glInterface::setUniform_texture("un_texture_" + std::to_string(i), in.textures[0]);

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);
}


void
idk::RenderEngine::_render_screenquad( GLuint shader, glFramebuffer &in )
{
    glInterface::unbindIdkFramebuffer(_screen_width, _screen_height);
    glInterface::useProgram(shader);

    for (int i=0; i < in.textures.size(); i++)
        glInterface::setUniform_texture("un_texture_" + std::to_string(i), in.textures[0]);

    gl::bindVertexArray(_quad_VAO);
    gl::drawArrays(GL_TRIANGLES, 0, 6);
}


int
idk::RenderEngine::createCamera()
{
    int camera_id = _camera_allocator.add();
    _camera_allocator.get(camera_id).aspect(_screen_width, _screen_height);
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
idk::RenderEngine::drawWireframe( GLuint shader_id, int model_id, Transform &transform )
{
    _wireframe_draw_queue[shader_id].push({model_id, transform});
}


void
idk::RenderEngine::_bind_material( idk::Material &material )
{
    glInterface::setUniform_texture("un_albedo_texture", material.albedo_gl_id);
    glInterface::setUniform_texture("un_specular_texture", material.specular_gl_id);
    glInterface::setUniform_float("un_specular_exponent", material.specular_exponent);
}


void
idk::RenderEngine::_draw_model( idk::Model &model, idk::Transform &transform )
{
    glm::mat4 model_mat = transform.modelMatrix();
    glInterface::setUniform_mat4("un_model", model_mat);

    glBindVertexArray(model.VAO);
    for (int i=0; i<model.meshes.size(); i++)
    {
        idk::Mesh &mesh = model.meshes[i];

        if (mesh.material_id != -1)
            _bind_material(modelManager().getMaterials().get(mesh.material_id));

        gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
        gl::drawElements(GL_TRIANGLES, mesh.vertex_indices.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);

    glInterface::freeTextureUnitIDs();
}


void
idk::RenderEngine::_draw_wireframe( idk::Model &model, idk::Transform &transform )
{
    glm::mat4 model_mat = transform.modelMatrix();
    idk::glInterface::setUniform_mat4("un_model", model_mat);

    gl::bindVertexArray(model.VAO);
    for (int i=0; i<model.meshes.size(); i++)
    {
        idk::Mesh &mesh = model.meshes[i];
        idk::gl::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
        idk::gl::drawElements(GL_TRIANGLES, mesh.vertex_indices.size(), GL_UNSIGNED_INT, 0);
    }
    gl::bindVertexArray(0);

    idk::glInterface::freeTextureUnitIDs();
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
    glInterface::bindIdkFramebuffer(_screen_width, _screen_height, _gb_geometry_buffer);

    for (auto &[shader_id, vec]: _model_draw_queue)
    {
        glInterface::useProgram(shader_id);

        glInterface::setUniform_int("un_num_pointlights", _pointlight_allocator.size());
        glInterface::setUniform_int("un_num_spotlights", _spotlight_allocator.size());

        idk::Camera &camera = getCamera();

        int count = 0;
        pointlights().for_each(
            [&count](lightsource::Point &pointlight)
            {
                idk::Transform &transform = pointlight.transform;
                std::string str = std::to_string(count);
                glInterface::setUniform_vec3("un_pointlights[" + str + "].ambient", pointlight.ambient);
                glInterface::setUniform_vec3("un_pointlights[" + str + "].diffuse", pointlight.diffuse);
                glInterface::setUniform_vec3("un_pointlights[" + str + "].position", pointlight.transform.position());

                glInterface::setUniform_float("un_pointlights[" + str + "].attenuation_constant", pointlight.attentuation_constant);
                glInterface::setUniform_float("un_pointlights[" + str + "].attentuation_linear", pointlight.attentuation_linear);
                glInterface::setUniform_float("un_pointlights[" + str + "].attentuation_quadratic", pointlight.attentuation_quadratic);

                count += 1;
            }
        );


        count = 0;
        spotlights().for_each(
            [&camera, &count](lightsource::Spot &spotlight)
            {
                idk::Transform &transform = spotlight.transform;
                std::string str = std::to_string(count);
                glInterface::setUniform_vec3("un_spotlights[" + str + "].ambient", spotlight.ambient);
                glInterface::setUniform_vec3("un_spotlights[" + str + "].diffuse", spotlight.diffuse);
                glInterface::setUniform_vec3("un_spotlights[" + str + "].position", transform.position());
                glInterface::setUniform_vec3("un_spotlights[" + str + "].direction", camera.front());

                glInterface::setUniform_float("un_spotlights[" + str + "].attenuation_constant", spotlight.attentuation_constant);
                glInterface::setUniform_float("un_spotlights[" + str + "].attentuation_linear", spotlight.attentuation_linear);
                glInterface::setUniform_float("un_spotlights[" + str + "].attentuation_quadratic", spotlight.attentuation_quadratic);
                
                glInterface::setUniform_float("un_spotlights[" + str + "].inner_cutoff", glm::radians(spotlight.inner_cutoff));
                glInterface::setUniform_float("un_spotlights[" + str + "].outer_cutoff", glm::radians(spotlight.outer_cutoff));

                count += 1;
            }
        );



        glm::mat4 view = camera.view();
        glm::mat4 proj = camera.projection();

        glInterface::setUniform_vec3("un_viewpos", camera.transform().position());
        glInterface::setUniform_mat4("un_view", view);
        glInterface::setUniform_mat4("un_projection", proj);

        for (auto &[model_id, transform]: vec)
        {
            _draw_model(
                modelManager().getModel(model_id),
                transform
            );
        }
        vec.clear();
    }
    _model_draw_queue.clear();



    for (auto &[shader_id, vec]: _wireframe_draw_queue)
    {
        glInterface::useProgram(shader_id);

        idk::Camera &camera = _camera_allocator.get(_active_camera_id);
        glm::mat4 view = camera.view();
        glm::mat4 proj = camera.projection();

        glInterface::setUniform_vec3("un_viewpos", camera.transform().position());
        glInterface::setUniform_mat4("un_view", view);
        glInterface::setUniform_mat4("un_projection", proj);

        for (auto &[model_id, transform]: vec)
        {
            _draw_wireframe(
                modelManager().getModel(model_id),
                transform
            );
        }
        vec.clear();
    }
    _wireframe_draw_queue.clear();



    gl::disable(GL_DEPTH_TEST, GL_CULL_FACE);
    _render_screenquad(_screenquad_shader, _gb_geometry_buffer, _screenquad_buffer);
    _render_screenquad(_colorgrade_shader, _screenquad_buffer, _colorgrade_buffer);
    _render_screenquad(_fxaa_shader, _colorgrade_buffer);
    gl::enable(GL_DEPTH_TEST, GL_CULL_FACE);

}


void
idk::RenderEngine::resize( int w, int h )
{
    _screen_width = w;
    _screen_height = h;

    glInterface::genIdkFramebuffer(w, h, _gb_geometry_buffer);
    glInterface::genIdkFramebuffer(w, h, _screenquad_buffer);
    glInterface::genIdkFramebuffer(w, h, _colorgrade_buffer);

    getCamera().aspect(w, h);
}

