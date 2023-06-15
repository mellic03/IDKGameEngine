#include "IDK_glInterface.h"
#include <fstream>
#include <sstream>


idk::glInterface::glInterface()
{
    for (GLint i = GL_TEXTURE0; i < IDK_GLINTERFACE_MAX_TEXTUREi; i++)
        _available_glTextureUnits.push(i);
}


bool
idk::glInterface::_line_has_include(std::string &line)
{
    size_t slash_pos = line.find("//");
    if (slash_pos != std::string::npos)
        line = line.substr(0, slash_pos);

    size_t include_pos = line.find("#include");
    
    if (include_pos == std::string::npos)
        return false;

    return true;
}


std::string
idk::glInterface::_parse_shader_include(std::string root, std::string includeline)
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
idk::glInterface::_parse_shader_source(std::string root, std::string glsl)
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
    return src;
}


GLuint
idk::glInterface::compileShaderProgram(std::string root, std::string vs, std::string fs)
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
idk::glInterface::loadTexture(std::string filepath)
{
    GLuint texture_id;
    SDL_Surface *img;
    img = IMG_Load(filepath.c_str());

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // if (useSRGB)
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
    // else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, 0);

    _textures[filepath] = texture_id;
}


void
idk::glInterface::genScreenBuffer(int width, int height, GLuint &FBO, GLuint &RBO, idk::vector<GLuint> &textures)
{
    glDeleteFramebuffers(1, &FBO);
    glDeleteRenderbuffers(1, &RBO);
    glDeleteTextures(textures.size(), &textures[0]);

    glGenFramebuffers(1, &FBO);
    glGenRenderbuffers(1, &RBO);
    glGenTextures(textures.size(), &textures[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    for (size_t i=0; i<textures.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, textures[i], 0);  
    }

    idk::vector<GLuint> attachments(textures.size());
    for (size_t i=0; i<textures.size(); i++)
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    glDrawBuffers(textures.size(), &attachments[0]);
     
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void
idk::glInterface::genScreenBuffer(int width, int height, idk::glInterface::ScreenBuffer &screenbuffer)
{
    genScreenBuffer(width, height, screenbuffer.FBO, screenbuffer.RBO, screenbuffer.textures);
}



GLuint
idk::glInterface::pop_glTextureUnitID()
{
    GLuint textureunit_id = _available_glTextureUnits.pop();
    _unavailable_glTextureUnits.push(textureunit_id);
    return textureunit_id;
}


void
idk::glInterface::free_glTextureUnitIDs()
{
    while (_unavailable_glTextureUnits.empty() == false)
        _available_glTextureUnits.push(_unavailable_glTextureUnits.pop());
}


void
idk::glInterface::bindShaderProgram(GLuint shader_id)
{
    free_glTextureUnitIDs();
    _active_shader_id = shader_id;
    GLCALL( glUseProgram(shader_id); )
}


void
idk::glInterface::bindScreenbuffer(int width, int height, idk::glInterface::ScreenBuffer &screen_buffer)
{
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, screen_buffer.FBO);
    GLCALL( glClearColor(1.0f, 1.0f, 1.0f, 1.0f); )
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void
idk::glInterface::bindMaterial(idk::Material &material)
{
    setUniform_texture("un_albedo_texture", material.albedo_texture);
    setUniform_texture("un_specular_texture", material.specular_texture);
}


void
idk::glInterface::draw_model(idk::Model &model, idk::transform &transform, idk::glUniforms &uniforms)
{
    glm::mat4 model_mat = transform.modelMatrix();
    setmat4("un_model", model_mat);

    for (size_t i=0; i<model.meshes.size(); i++)
    {
        idk::Mesh &mesh = model.meshes[i];
        idk::Material &material = _material_allocator.get(mesh.material_id);

        bindMaterial(material);

        GLCALL( glBindVertexArray(mesh.VAO); )

        GLCALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.IBOS[i]); )
        GLCALL( glDrawElements(
            GL_TRIANGLES,
            mesh.vertex_indices.size(),
            GL_UNSIGNED_INT,
            (void *)0
        ); )
    }

    free_glTextureUnitIDs();
}



void
idk::glInterface::setint(const char *name, int i)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform1i(loc, i);
    )
}

void
idk::glInterface::setfloat(const char *name, float f)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform1f(loc, f);
    )
}

void
idk::glInterface::setvec2(const char *name, glm::vec2 v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform2fv(loc, 1, glm::value_ptr(v));
    )
}

void
idk::glInterface::setvec3(const char *name, glm::vec3 v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform3fv(loc, 1, glm::value_ptr(v));
    )
}

void
idk::glInterface::setvec4(const char *name, glm::vec4 v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform4fv(loc, 1, glm::value_ptr(v));
    )
}

void
idk::glInterface::setmat3(const char *name, glm::mat3 m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    )
}

void
idk::glInterface::setmat4(const char *name, glm::mat4 m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    )
}



void
idk::glInterface::setvec2(const char *name, const float *v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform2fv(loc, 1, v);
    )
}


void
idk::glInterface::setvec3(const char *name, float *v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform3fv(loc, 1, v);
    )
}


void
idk::glInterface::setvec4(const char *name, float *v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform4fv(loc, 1, v);
    )
}


void
idk::glInterface::setmat3(const char *name, float *m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniformMatrix3fv(loc, 1, GL_FALSE, m);
    )
}


void
idk::glInterface::setmat4(const char *name, float *m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, m);
    )
}


void
idk::glInterface::setUniform_texture(std::string name, GLuint texture_id)
{
    GLuint texture_unit = pop_glTextureUnitID();
    GLCALL( glActiveTexture(texture_unit); )
    GLCALL( glBindTexture(GL_TEXTURE_2D, texture_id); )
    setint(name.c_str(), texture_unit - GL_TEXTURE0);
}


