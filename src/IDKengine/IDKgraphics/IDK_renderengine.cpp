#include "IDK_renderengine.h"

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}


void
idk::RenderEngine::_init_screenquad()
{
    float quad_vertices[30] = {
      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
      -1.0f, -1.0f,  -0.999f,  0.0f,  0.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,

      -1.0f,  1.0f,  -0.999f,  0.0f,  1.0f,
       1.0f, -1.0f,  -0.999f,  1.0f,  0.0f,
       1.0f,  1.0f,  -0.999f,  1.0f,  1.0f
    };

    // Send screen quad to GPU
    GLCALL( glGenVertexArrays(1, &_quad_VAO); )
    GLCALL( glGenBuffers(1, &_quad_VBO); )
    GLCALL( glBindVertexArray(_quad_VAO); )
    GLCALL( glBindBuffer(GL_ARRAY_BUFFER, _quad_VBO); )
    GLCALL( glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW); )

    GLCALL( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); )
    GLCALL( glEnableVertexAttribArray(0); )
    GLCALL( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); )
    GLCALL( glEnableVertexAttribArray(2); )
}


idk::RenderEngine::RenderEngine(size_t w, size_t h):
_screen_width(w), _screen_height(h),
_gbuffer_geometrypass_buffer(4), _screenquad_buffer(1), _colorgrade_buffer(1)
{
    _init_SDL_OpenGL(_screen_width, _screen_height);
    _init_screenquad();

    idk::glInterface &gl = _gl_interface;

    CUBE_PRIMITIVE = _model_allocator.add(Model(IDK_primitive_cube_raw, "", gl.materials(), gl.textures()));
    SPHERE_PRIMITIVE = _model_allocator.add(Model(IDK_primitive_icosphere_raw, "", gl.materials(), gl.textures()));


    gl.genFramebuffer(_screen_width, _screen_height, _gbuffer_geometrypass_buffer);
    gl.genFramebuffer(_screen_width, _screen_height, _screenquad_buffer);
    gl.genFramebuffer(_screen_width, _screen_height, _colorgrade_buffer);
    _screenquad_shader = gl.compileShaderProgram("assets/shaders/", "screenquad.vs", "screenquad.fs");
    _colorgrade_shader = gl.compileShaderProgram("assets/shaders/", "screenquad.vs", "colorgrade.fs");
    _fxaa_shader = gl.compileShaderProgram("assets/shaders/", "screenquad.vs", "fxaa.fs");
    primitive_shader = gl.compileShaderProgram("assets/shaders/", "vsin_pos_only.vs", "solid.fs");
    wireframe_shader = gl.compileShaderProgram("assets/shaders/", "vsin_pos_only.vs", "wireframe.fs");
}


void
idk::RenderEngine::_render_screenquad()
{
    idk::glInterface &gl = _gl_interface;
    
    gl.bindFramebuffer(_screen_width, _screen_height, _screenquad_buffer);
    gl.bindShaderProgram(_screenquad_shader);
    gl.setUniform_texture("un_screen_texture", _gbuffer_geometrypass_buffer.textures[0]);
    gl.drawVertices(_quad_VAO, 6);
}


void
idk::RenderEngine::_colorgrade_screenquad()
{
    idk::glInterface &gl = _gl_interface;
    
    gl.bindFramebuffer(_screen_width, _screen_height, _colorgrade_buffer);
    gl.bindShaderProgram(_colorgrade_shader);
    gl.setUniform_texture("un_screen_texture", _screenquad_buffer.textures[0]);
    gl.drawVertices(_quad_VAO, 6);
}


void
idk::RenderEngine::_fxaa_screenquad()
{
    idk::glInterface &gl = _gl_interface;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    gl.bindShaderProgram(_fxaa_shader);
    gl.setUniform_texture("un_screen_texture", _colorgrade_buffer.textures[0]);
    gl.drawVertices(_quad_VAO, 6);

}


uint
idk::RenderEngine::createCamera()
{
    uint camera_id = _camera_allocator.add();
    _camera_allocator.get(camera_id).aspect(_screen_width, _screen_height);
    return camera_id;
}


uint
idk::RenderEngine::createPointLight()
{
    return _pointlight_allocator.add();
}


void
idk::RenderEngine::drawModel( uint model_id, Transform &transform )
{
    _model_draw_queue[_active_shader_id].push({model_id, transform, glUniforms()});
}


void
idk::RenderEngine::drawModel( GLuint shader_id, uint primitive_id, Transform &transform )
{
    _model_draw_queue[shader_id].push({primitive_id, transform, glUniforms()});
}


void
idk::RenderEngine::drawWireframe( GLuint shader_id, uint primitive_id, Transform &transform )
{
    _wireframe_draw_queue[shader_id].push({primitive_id, transform, glUniforms()});
}


void
idk::RenderEngine::setUniform_vec3( GLuint shader_id, std::string name, glm::vec3 v )
{
    _model_draw_queue[shader_id].back().third.setvec3(name, v);
}


void
idk::RenderEngine::loadTextures( std::string root )
{
    using namespace std;

    filesystem::path rootpath(root);
    for (auto const &dir_entry: filesystem::recursive_directory_iterator{rootpath})
    {
        _gl_interface.loadTexture(dir_entry.path());
    }
}


uint
idk::RenderEngine::loadOBJ(std::string root, std::string obj, std::string mtl)
{
    idk::glInterface &gl = _gl_interface;
    uint model_id = _model_allocator.add(idk::Model(root, obj, mtl, gl.materials(), gl.textures()));
    return model_id;
}


void
idk::RenderEngine::beginFrame()
{
    SDL_GL_SwapWindow(_SDL_window);

    GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, 0); )
    GLCALL( glClearColor(0.0f, 0.0f, 0.0f, 1.0f); )
    GLCALL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); )
}


void
idk::RenderEngine::endFrame()
{
    idk::Camera &camera = _camera_allocator.get(_active_camera_id);

    idk::RenderEngine &ren = *this;

    idk::glInterface &gl = _gl_interface;
    gl.bindFramebuffer(_screen_width, _screen_height, _gbuffer_geometrypass_buffer);

    auto &transform_allocator = _transform_allocator;

    for (auto &[shader_id, vec]: _model_draw_queue)
    {
        gl.bindShaderProgram(shader_id);

        gl.setint("un_num_pointlights", _pointlight_allocator.size());

        int count = 0;
        pointlights().for_each(
            [&ren, &gl, &count](lightsource::Point &pointlight)
            {
                idk::Transform &transform = pointlight.transform;
                std::string str = std::to_string(count);
                gl.setvec3("un_pointlights[" + str + "].ambient", pointlight.ambient);
                gl.setvec3("un_pointlights[" + str + "].diffuse", pointlight.diffuse);
                gl.setvec3("un_pointlights[" + str + "].position", glm::vec3(0.0f, 40.0f, 20.0f));

                gl.setfloat("un_pointlights[" + str + "].attenuation_constant", pointlight.attentuation_constant);
                gl.setfloat("un_pointlights[" + str + "].attentuation_linear", pointlight.attentuation_linear);
                gl.setfloat("un_pointlights[" + str + "].attentuation_quadratic", pointlight.attentuation_quadratic);

                count += 1;
            }
        );

        glm::mat4 view = camera.view();
        glm::mat4 proj = camera.projection();

        gl.setvec3("un_viewpos", camera.transform().position());
        gl.setmat4("un_view", view);
        gl.setmat4("un_projection", proj);

        for (auto [model_id, transform, glUniform]: vec)
        {
            gl.drawModel(
                _model_allocator.get(model_id),
                transform,
                glUniform
            );
        }
        vec.clear();
    }
    _model_draw_queue.clear();


    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    for (auto &[shader_id, vec]: _wireframe_draw_queue)
    {
        gl.bindShaderProgram(shader_id);

        glm::mat4 view = camera.view();
        glm::mat4 proj = camera.projection();

        gl.setvec3("un_viewpos", camera.transform().position());
        gl.setmat4("un_view", view);
        gl.setmat4("un_projection", proj);

        for (auto [model_id, transform, glUniform]: vec)
        {
            gl.drawModel(
                _model_allocator.get(model_id),
                transform,
                glUniform
            );
        }
        vec.clear();
    }
    _wireframe_draw_queue.clear();
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


    gl.disable(GL_DEPTH_TEST, GL_CULL_FACE);
    _render_screenquad();
    _colorgrade_screenquad();
    _fxaa_screenquad();
    gl.enable(GL_DEPTH_TEST, GL_CULL_FACE);
}


void
idk::RenderEngine::resize( int w, int h )
{
    _screen_width = w;
    _screen_height = h;

    idk::glInterface &gl = _gl_interface;
    gl.genFramebuffer(w, h, _gbuffer_geometrypass_buffer);
    gl.genFramebuffer(w, h, _screenquad_buffer);
    gl.genFramebuffer(w, h, _colorgrade_buffer);

    getActiveCamera().aspect(w, h);
}

