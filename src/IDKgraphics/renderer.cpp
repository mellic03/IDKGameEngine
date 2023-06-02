#include "renderer.h"

#include <iostream>


IDK::RenderEngine::RenderEngine(size_t w, size_t h)
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



GLuint
IDK::RenderEngine::_load_shader(std::string root, std::string vs, std::string fs)
{
    std::ifstream instream;
    std::string line;

    std::string vert_src, frag_src;


    instream = std::ifstream(root + vs);
    while (getline(instream, line))
        vert_src += line + "\n";

    instream = std::ifstream(root + fs);
    while (getline(instream, line))
        frag_src += line + "\n";

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

    // _shader_allocator.add(program_id);

    return program_id;
}


void
IDK::RenderEngine::_use_shader(Shader &shader)
{
    GLCALL( glUseProgram(shader.id()); )
}

void
IDK::RenderEngine::useShader(int id)
{
    _active_shader_id = id;
}

void
IDK::RenderEngine::loadShader(std::string path)
{

}


GLuint
IDK::RenderEngine::_gen_texture(std::string path)
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


void
IDK::RenderEngine::_bind_material(int id)
{
    IDK::Material &material = _material_allocator.get(id);
}


void
IDK::RenderEngine::_draw_mesh(IDK::Mesh &mesh)
{
    _bind_material(mesh.material_id);

    for (GLuint ibo: mesh.IBOS)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        
    }
}


void
IDK::RenderEngine::_draw_model(int id, IDK::transform *transform)
{
    IDK::Model &model = _model_allocator.get(id);
    
    GLCALL( glBindVertexArray(model.VAO); )
    for (IDK::Mesh &mesh: model.meshes)
        _draw_mesh(mesh);
}


void
IDK::RenderEngine::drawModel(int id, IDK::transform &transform)
{

}


void
IDK::RenderEngine::_draw_to_screen(GLuint texture_id)
{
    GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, 0); )
    GLCALL( glActiveTexture(GL_TEXTURE0); )
    GLCALL( glBindTexture(GL_TEXTURE_2D, texture_id); )
}


void
IDK::RenderEngine::beginFrame()
{
    GLCALL( glClearColor(0.0f, 0.0f, 0.0f, 1.0f); )
    GLCALL( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); )
}


void
IDK::RenderEngine::endFrame()
{
    _use_shader(_screenquad_shader);


    GLCALL( glDisable(GL_DEPTH_TEST); )
    GLCALL( glDisable(GL_CULL_FACE); )

    GLCALL( glBindVertexArray(_quad_VAO); )
    GLCALL( glDrawArrays(GL_TRIANGLES, 0, 6); )
    GLCALL( glBindVertexArray(0); )

    SDL_GL_SwapWindow(_SDL_window);
}

