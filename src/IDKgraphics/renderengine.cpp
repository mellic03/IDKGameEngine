#include "renderengine.h"

#include <iostream>


idk::RenderEngine::RenderEngine(size_t w, size_t h)
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
        1000,
        1000,
        SDL_WINDOW_OPENGL
        // SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
    );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


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


    // Generate screen quad vertex array
    GLCALL( glGenVertexArrays(1, &_quad_VAO); )
    GLCALL( glGenBuffers(1, &_quad_VBO); )
    GLCALL( glBindVertexArray(_quad_VAO); )
    GLCALL( glBindBuffer(GL_ARRAY_BUFFER, _quad_VBO); )
    GLCALL( glBufferData(GL_ARRAY_BUFFER, sizeof(_quad_vertices), &_quad_vertices, GL_STATIC_DRAW); )

    GLCALL( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); )
    GLCALL( glEnableVertexAttribArray(0); )
    GLCALL( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); )
    GLCALL( glEnableVertexAttribArray(2); )


    _screenquad_shader = Shader("assets/shaders/", "screenquad.vs", "screenquad.fs");
}


bool
idk::RenderEngine::_line_has_include(std::string &line)
{
    size_t hash_pos = line.find("#include");
    size_t slash_pos = line.find("//");
    
    if (hash_pos == std::string::npos)
        return false;

    if (slash_pos == std::string::npos || slash_pos > hash_pos)
        return true;
    
    return false;
}


std::string
idk::RenderEngine::_parse_shader_include(std::string root, std::string includeline)
{
    std::string src;

    std::istringstream iss(includeline);
    std::string line, includepath;
    iss >> line >> includepath;
    
    includepath = includepath.substr(1, includepath.size()-2);
    includepath = root + includepath;

    std::ifstream instream(includepath);
    while (getline(instream, line))
    {
        if (_line_has_include(line))
            src += _parse_shader_include(root, line);
        else
            src += line + '\n';
    }

    return src;
}


std::string
idk::RenderEngine::_parse_shader_source(std::string root, std::string glsl)
{
    std::string src;

    std::ifstream instream(root + glsl);
    std::string line;

    while (getline(instream, line))
    {
        if (_line_has_include(line))
            src += _parse_shader_include(root, line);
        else
            src += line + "\n";
    }

    std::cout << src << std::endl;

    return src;
}


GLuint
idk::RenderEngine::_load_shader(std::string root, std::string vs, std::string fs)
{
    std::ifstream instream;
    std::string line;

    std::string vert_src = _parse_shader_source(root, vs);
    std::string frag_src = _parse_shader_source(root, fs);

    auto compile_shader = [](std::string &src, GLenum type)
    {
        const char *str = src.c_str();
        GLuint shader_id = glCreateShader(type);
        glShaderSource(shader_id, 1, &str, nullptr);
        glCompileShader(shader_id);

        int result;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
            char *message = (char *)alloca(length * sizeof(char));
            glGetShaderInfoLog(shader_id, length, &length, message);

            std::cout << "Failed to compile "
                      << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                      << " shader\n" << std::endl; 
            std::cout << message << std::endl;
            exit(1);
        }

        return shader_id;
    };

    GLuint vert_id = compile_shader(vert_src, GL_VERTEX_SHADER);
    GLuint frag_id = compile_shader(frag_src, GL_FRAGMENT_SHADER);

    GLuint program_id = glCreateProgram();
    GLCALL( glAttachShader(program_id, vert_id); )
    GLCALL( glAttachShader(program_id, frag_id); )
    GLCALL( glLinkProgram(program_id); )
    GLCALL( glValidateProgram(program_id); )

    GLCALL( glDeleteShader(vert_id); )
    GLCALL( glDeleteShader(frag_id); )

    return program_id;
}


void
idk::RenderEngine::_bind_shader(GLuint shader_id)
{
    GLCALL( glUseProgram(shader_id); )
}


GLuint
idk::RenderEngine::compileShaderProgram(std::string root, std::string vs, std::string fs)
{
    return _load_shader(root, vs, fs);
}


void
idk::RenderEngine::_set_uniform_int(const char *name, int i)
{
    GLuint loc = glGetUniformLocation(_active_shader_id, name);
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform1i(loc, i);
    )
}

void
idk::RenderEngine::_set_uniform_float(const char *name, float f)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform1f(loc, f);
    )
}

void
idk::RenderEngine::_set_uniform_vec2(const char *name, idk::vec2 &v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform2fv(loc, 1, idk::value_ptr(v));
    )
}

void
idk::RenderEngine::_set_uniform_vec3(const char *name, idk::vec3 &v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform3fv(loc, 1, idk::value_ptr(v));
    )
}

void
idk::RenderEngine::_set_uniform_vec4(const char *name, idk::vec4 &v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform4fv(loc, 1, idk::value_ptr(v));
    )
}

void
idk::RenderEngine::_set_uniform_mat3(const char *name, idk::mat3 &m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniformMatrix3fv(loc, 1, GL_FALSE, idk::value_ptr(m));
    )
}

void
idk::RenderEngine::_set_uniform_mat4(const char *name, idk::mat4 &m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, idk::value_ptr(m));
    )
}



GLuint
idk::RenderEngine::_gen_texture(std::string path)
{
    GLuint texture_id;

    unsigned char *data;
    std::ifstream instream(path);

    if (instream.good())
    {
        // data = load_img()
    }


    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // if (useSRGB)
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // else
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, 0);


    // _texture_allocator.add(texture_id);

    return texture_id;
}


GLuint
idk::RenderEngine::_pop_gltextureunit_id()
{
    GLuint textureunit_id = _gltu_available.pop();
    _gltu_unavailable.push(textureunit_id);
    return textureunit_id;
}


void
idk::RenderEngine::_free_gltextureunits()
{
    while (_gltu_unavailable.empty() == false)
        _gltu_available.push(_gltu_unavailable.pop());
}


void
idk::RenderEngine::_bind_texture(GLuint texture_id)
{
    GLCALL( glActiveTexture(_pop_gltextureunit_id()); )
    GLCALL( glBindTexture(GL_TEXTURE_2D, texture_id); )
}


void
idk::RenderEngine::_bind_material(int material_id)
{
    idk::Material &material = _material_allocator.get(material_id);
    _bind_texture(material.texture_id);
}


void
idk::RenderEngine::_draw_mesh(idk::Mesh &mesh)
{
    _bind_material(mesh.material_id);

    for (GLuint ibo: mesh.IBOS)
    {
        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); )
    }
}


void
idk::RenderEngine::_draw_model(int model_id, int transform_id)
{
    idk::Model &model = _model_allocator.get(model_id);
    idk::transform &transform = _transform_allocator.get(transform_id);
    
    // GLCALL( glBindVertexArray(model.VAO); )
    // for (idk::Mesh &mesh: model.meshes)
    //     _draw_mesh(mesh);
}


void
idk::RenderEngine::drawModel(int model_id, int transform_id)
{
    _model_draw_queue[_active_shader_id].push({model_id, transform_id});
}


void
idk::RenderEngine::_draw_to_screen(GLuint texture_id)
{
    GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, 0); )
    GLCALL( glActiveTexture(GL_TEXTURE0); )
    GLCALL( glBindTexture(GL_TEXTURE_2D, texture_id); )
}


int
idk::RenderEngine::loadOBJ(std::string root, std::string obj, std::string mtl)
{
    return _model_allocator.add(idk::Model(root, obj, mtl));
}


void
idk::RenderEngine::beginFrame()
{
    _free_gltextureunits();

    GLCALL( glClearColor(0.0f, 0.0f, 0.0f, 1.0f); )
    GLCALL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); )
}


void
idk::RenderEngine::endFrame()
{
    for (auto &[shader_id, vec]: _model_draw_queue)
    {
        _bind_shader(shader_id);
        for (auto [model_id, transform_id]: vec)
            _draw_model(model_id, transform_id);
        vec.clear();
    }


    _free_gltextureunits();

    // Draw final texture to the screen quad

    // _bind_shader(_screenquad_shader);

    // GLCALL( glDisable(GL_DEPTH_TEST); )
    // GLCALL( glDisable(GL_CULL_FACE); )

    // GLCALL( glBindVertexArray(_quad_VAO); )
    // GLCALL( glDrawArrays(GL_TRIANGLES, 0, 6); )
    // GLCALL( glBindVertexArray(0); )

    // SDL_GL_SwapWindow(_SDL_window);
}

