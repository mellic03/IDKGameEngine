#include "idk_glInterface.h"
#include <fstream>
#include <sstream>
#include <iostream>

static    GLuint                  _active_shader_id;
static    idk::vector<GLuint>     _available_glTextureUnits;
static    idk::vector<GLuint>     _unavailable_glTextureUnits;


void
idk::glInterface::init()
{
    for (GLint i = GL_TEXTURE0; i < IDK_GLINTERFACE_MAX_TEXTUREi; i++)
        _available_glTextureUnits.push(i);
}



static bool
_line_has_include( std::string &line )
{
    size_t slash_pos = line.find("//");
    if (slash_pos != std::string::npos)
        line = line.substr(0, slash_pos);

    size_t include_pos = line.find("#include");
    
    if (include_pos == std::string::npos)
        return false;

    return true;
}


static std::string
_parse_shader_include( std::string root, std::string includeline )
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


static std::string
_parse_shader_source( std::string root, std::string glsl )
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
idk::glInterface::compileProgram( std::string root, std::string vs, std::string fs )
{
    std::ifstream instream;
    std::string line;

    std::string vert_src = _parse_shader_source(root, vs);
    std::string frag_src = _parse_shader_source(root, fs);

    auto compile_shader = [](std::string &src, GLenum type)
    {
        const char *str = src.c_str();
        GLuint shader_id = glCreateShader(type);
        GLCALL( glShaderSource(shader_id, 1, &str, nullptr); )
        GLCALL( glCompileShader(shader_id); )

        int result;
        GLCALL( glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result); )
        if (result == GL_FALSE)
        {
            int length;
            GLCALL( glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length); )
            char *message = (char *)alloca(length * sizeof(char));
            GLCALL( glGetShaderInfoLog(shader_id, length, &length, message); )

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


GLuint
idk::glInterface::loadTexture( std::string path, int w, int h, uint8_t* &data, bool srgb )
{
    GLuint texture_id;

    GLCALL( glGenTextures(1, &texture_id); )
    GLCALL( glBindTexture(GL_TEXTURE_2D, texture_id); )

    GLCALL( glPixelStorei(GL_UNPACK_ALIGNMENT, 1); )

    if (srgb)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    GLCALL( glGenerateMipmap(GL_TEXTURE_2D); )

    GLCALL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); )
    GLCALL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); )
    GLCALL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); )
    GLCALL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); )

    GLCALL( glBindTexture(GL_TEXTURE_2D, 0); )

    return texture_id;
}


void
idk::glInterface::genIdkFramebuffer( int width, int height, GLuint &FBO, GLuint &RBO, idk::vector<GLuint> &textures )
{
    GLCALL( glDeleteFramebuffers(1, &FBO); )
    GLCALL( glDeleteRenderbuffers(1, &RBO); )
    GLCALL( glDeleteTextures(textures.size(), &textures[0]); )

    GLCALL( glGenFramebuffers(1, &FBO); )
    GLCALL( glGenRenderbuffers(1, &RBO); )
    GLCALL( glGenTextures(textures.size(), &textures[0]); )

    GLCALL( glBindFramebuffer(GL_FRAMEBUFFER, FBO); )

    for (size_t i=0; i<textures.size(); i++)
    {
        GLCALL( glBindTexture(GL_TEXTURE_2D, textures[i]); )
        GLCALL( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL); )
        GLCALL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); )
        GLCALL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); )
        GLCALL( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, textures[i], 0);   )
    }

    idk::vector<GLuint> attachments(textures.size());
    for (size_t i=0; i<textures.size(); i++)
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    GLCALL( glDrawBuffers(textures.size(), &attachments[0]); )
     
    GLCALL( glBindRenderbuffer(GL_RENDERBUFFER, RBO); )
    GLCALL( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); )
    GLCALL( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO); )

    GLCALL( glBindRenderbuffer(GL_RENDERBUFFER, 0); )
    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);
}


void
idk::glInterface::genIdkFramebuffer(int width, int height, glFramebuffer &fb)
{
    genIdkFramebuffer(width, height, fb.FBO, fb.RBO, fb.textures);
}


GLuint
idk::glInterface::popTextureUnitID()
{
    GLuint textureunit_id = _available_glTextureUnits.pop();
    _unavailable_glTextureUnits.push(textureunit_id);
    return textureunit_id;
}


void
idk::glInterface::freeTextureUnitIDs()
{
    while (_unavailable_glTextureUnits.empty() == false)
        _available_glTextureUnits.push(_unavailable_glTextureUnits.pop());
}


void
idk::glInterface::useProgram(GLuint shader_id)
{
    freeTextureUnitIDs();
    _active_shader_id = shader_id;
    GLCALL( glUseProgram(_active_shader_id); )
}


void
idk::glInterface::bindIdkFramebuffer( int width, int height, glFramebuffer &framebuffer )
{
    gl::viewport(0, 0, width, height);
    gl::bindFramebuffer(GL_FRAMEBUFFER, framebuffer.FBO);
    gl::clearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void
idk::glInterface::unbindIdkFramebuffer( int width, int height )
{
    gl::viewport(0, 0, width, height);
    gl::bindFramebuffer(GL_FRAMEBUFFER, 0);
    gl::clearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void
idk::glInterface::setUniform_int(std::string name, int i)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name.c_str());
        glUniform1i(loc, i);
    )
}

void
idk::glInterface::setUniform_float(std::string name, float f)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name.c_str());
        glUniform1f(loc, f);
    )
}

void
idk::glInterface::setUniform_vec2(std::string name, glm::vec2 v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name.c_str());
        glUniform2fv(loc, 1, glm::value_ptr(v));
    )
}

void
idk::glInterface::setUniform_vec3(std::string name, glm::vec3 v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name.c_str());
        glUniform3fv(loc, 1, glm::value_ptr(v));
    )
}

void
idk::glInterface::setUniform_vec4(std::string name, glm::vec4 v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name.c_str());
        glUniform4fv(loc, 1, glm::value_ptr(v));
    )
}

void
idk::glInterface::setUniform_mat3(std::string name, glm::mat3 m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name.c_str());
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    )
}

void
idk::glInterface::setUniform_mat4(std::string name, glm::mat4 m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    )
}

void
idk::glInterface::setUniform_texture(std::string name, GLuint texture_id)
{
    GLuint texture_unit = popTextureUnitID();
    gl::activeTexture(texture_unit);
    gl::bindTexture(GL_TEXTURE_2D, texture_id);
    setUniform_int(name.c_str(), texture_unit - GL_TEXTURE0);
}



void
idk::glInterface::setUniform_int(GLint loc, int i)
{
    GLCALL( glUniform1i(loc, i); )
}

void
idk::glInterface::setUniform_float(GLint loc, float f)
{
    GLCALL( glUniform1f(loc, f); )
}

void
idk::glInterface::setUniform_vec2(GLint loc, glm::vec2 v)
{
    GLCALL( glUniform2fv(loc, 1, glm::value_ptr(v)); )
}

void
idk::glInterface::setUniform_vec3(GLint loc, glm::vec3 v)
{
    GLCALL( glUniform3fv(loc, 1, glm::value_ptr(v)); )
}

void
idk::glInterface::setUniform_vec4(GLint loc, glm::vec4 v)
{
    GLCALL( glUniform4fv(loc, 1, glm::value_ptr(v)); )
}

void
idk::glInterface::setUniform_mat3(GLint loc, glm::mat3 m)
{
    GLCALL( glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m)); )
}

void
idk::glInterface::setUniform_mat4(GLint loc, glm::mat4 m)
{
    GLCALL( glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m)); )
}


void
idk::glInterface::setUniform_texture(GLint loc, GLuint texture_id)
{
    GLuint texture_unit = popTextureUnitID();
    gl::activeTexture(texture_unit);
    gl::bindTexture(GL_TEXTURE_2D, texture_id);
    setUniform_int(loc, texture_unit - GL_TEXTURE0);
}

