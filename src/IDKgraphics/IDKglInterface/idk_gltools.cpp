#include "idk_gltools.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

static    GLuint                  m_active_shader_id;
static    idk::vector<GLuint>     m_available_glTextureUnits;
static    idk::vector<GLuint>     m_unavailable_glTextureUnits;


void
idk::gltools::init()
{
    for (GLint i = GL_TEXTURE0; i < IDK_GLINTERFACE_MAX_TEXTUREi; i++)
        m_available_glTextureUnits.push(i);
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
idk::gltools::compileProgram( std::string root, std::string vs, std::string fs )
{
    std::ifstream instream;
    std::string line;

    std::string vert_src = _parse_shader_source(root, vs);
    std::string frag_src = _parse_shader_source(root, fs);

    auto compile_shader = [](std::string &src, GLenum type, std::string root, std::string stem)
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

            std::cout << "File " << root + stem << std::endl;

            std::cout << "Failed to compile "
                      << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                      << " shader\n" << std::endl; 
            std::cout << message << std::endl;
            exit(1);
        }

        return shader_id;
    };

    GLuint vert_id = compile_shader(vert_src, GL_VERTEX_SHADER, root, vs);
    GLuint frag_id = compile_shader(frag_src, GL_FRAGMENT_SHADER, root, fs);

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
idk::gltools::loadTexture( int w, int h, uint32_t *data, bool srgb )
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



GLuint
idk::gltools::popTextureUnitID()
{
    #ifdef IDK_DEBUG
    if (m_available_glTextureUnits.size() == 0)
    {
        std::cout << "Ruh roh, "
                  << "m_available_glTextureUnits.size() == 0"
                  << std::endl;
        exit(1);
    }
    #endif

    GLuint textureunit_id = m_available_glTextureUnits.pop();
    m_unavailable_glTextureUnits.push(textureunit_id);
    return textureunit_id;
}


void
idk::gltools::freeTextureUnitIDs()
{
    while (m_unavailable_glTextureUnits.empty() == false)
        m_available_glTextureUnits.push(m_unavailable_glTextureUnits.pop());
}


void
idk::gltools::useProgram(GLuint shader_id)
{
    freeTextureUnitIDs();
    m_active_shader_id = shader_id;
    gl::useProgram(m_active_shader_id);
}


void
idk::gltools::setUniform_int(std::string name, int i)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(m_active_shader_id, name.c_str());
        glUniform1i(loc, i);
    )
}

void
idk::gltools::setUniform_float(std::string name, float f)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(m_active_shader_id, name.c_str());
        glUniform1f(loc, f);
    )
}

void
idk::gltools::setUniform_vec2(std::string name, glm::vec2 v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(m_active_shader_id, name.c_str());
        glUniform2fv(loc, 1, glm::value_ptr(v));
    )
}

void
idk::gltools::setUniform_vec3(std::string name, glm::vec3 v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(m_active_shader_id, name.c_str());
        glUniform3fv(loc, 1, glm::value_ptr(v));
    )
}

void
idk::gltools::setUniform_vec4(std::string name, glm::vec4 v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(m_active_shader_id, name.c_str());
        glUniform4fv(loc, 1, glm::value_ptr(v));
    )
}

void
idk::gltools::setUniform_mat3(std::string name, glm::mat3 m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(m_active_shader_id, name.c_str());
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    )
}

void
idk::gltools::setUniform_mat4(std::string name, glm::mat4 m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(m_active_shader_id, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    )
}

void
idk::gltools::setUniform_texture(std::string name, GLuint texture_id)
{
    GLuint texture_unit = popTextureUnitID();
    gl::activeTexture(texture_unit);
    gl::bindTexture(GL_TEXTURE_2D, texture_id);
    setUniform_int(name.c_str(), texture_unit - GL_TEXTURE0);
}


void
idk::gltools::setUniform_texture3D(std::string name, GLuint texture_id)
{
    GLuint texture_unit = popTextureUnitID();
    gl::activeTexture(texture_unit);
    gl::bindTexture(GL_TEXTURE_3D, texture_id);
    setUniform_int(name.c_str(), texture_unit - GL_TEXTURE0);
}


void
idk::gltools::setUniform_int(GLint loc, int i)
{
    GLCALL( glUniform1i(loc, i); )
}

void
idk::gltools::setUniform_float(GLint loc, float f)
{
    GLCALL( glUniform1f(loc, f); )
}

void
idk::gltools::setUniform_vec2(GLint loc, glm::vec2 v)
{
    GLCALL( glUniform2fv(loc, 1, glm::value_ptr(v)); )
}

void
idk::gltools::setUniform_vec3(GLint loc, glm::vec3 v)
{
    GLCALL( glUniform3fv(loc, 1, glm::value_ptr(v)); )
}

void
idk::gltools::setUniform_vec4(GLint loc, glm::vec4 v)
{
    GLCALL( glUniform4fv(loc, 1, glm::value_ptr(v)); )
}

void
idk::gltools::setUniform_mat3(GLint loc, glm::mat3 m)
{
    GLCALL( glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m)); )
}

void
idk::gltools::setUniform_mat4(GLint loc, glm::mat4 m)
{
    GLCALL( glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m)); )
}


void
idk::gltools::setUniform_texture(GLint loc, GLuint texture_id)
{
    GLuint texture_unit = popTextureUnitID();
    gl::activeTexture(texture_unit);
    gl::bindTexture(GL_TEXTURE_2D, texture_id);
    setUniform_int(loc, texture_unit - GL_TEXTURE0);
}

