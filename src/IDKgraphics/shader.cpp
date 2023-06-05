#include "shader.h"
#include <iostream>

idk::Shader::Shader(std::string dir, std::string vs, std::string fs)
{
    std::ifstream instream;
    std::string line;

    instream = std::ifstream(dir + vs);
    while (getline(instream, line))
        _vert_src += line + "\n";

    instream = std::ifstream(dir + fs);
    while (getline(instream, line))
        _frag_src += line + "\n";

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

    GLuint vert_id = compile_shader(_vert_src, GL_VERTEX_SHADER);
    GLuint frag_id = compile_shader(_frag_src, GL_FRAGMENT_SHADER);

    _program_id = glCreateProgram();
    GLCALL( glAttachShader(_program_id, vert_id); )
    GLCALL( glAttachShader(_program_id, frag_id); )
    GLCALL( glLinkProgram(_program_id); )
    GLCALL( glValidateProgram(_program_id); )

    GLCALL( glDeleteShader(vert_id); )
    GLCALL( glDeleteShader(frag_id); )
}


void idk::Shader::setint(const char *name, int i)
{
    GLuint loc = glGetUniformLocation(_program_id, name);
    GLCALL(
        GLuint loc = glGetUniformLocation(_program_id, name);
        glUniform1i(loc, i);
    )
}

void idk::Shader::setfloat(const char *name, float f)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_program_id, name);
        glUniform1f(loc, f);
    )
}

void idk::Shader::setvec2(const char *name, idk::vec2 &v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_program_id, name);
        glUniform2fv(loc, 1, idk::value_ptr(v));
    )
}

void idk::Shader::setvec3(const char *name, idk::vec3 &v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_program_id, name);
        glUniform3fv(loc, 1, idk::value_ptr(v));
    )
}

void idk::Shader::setvec4(const char *name, idk::vec4 &v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_program_id, name);
        glUniform4fv(loc, 1, idk::value_ptr(v));
    )
}

void idk::Shader::setmat3(const char *name, idk::mat3 &m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_program_id, name);
        glUniformMatrix3fv(loc, 1, GL_FALSE, idk::value_ptr(m));
    )
}

void idk::Shader::setmat4(const char *name, idk::mat4 &m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_program_id, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, idk::value_ptr(m));
    )
}


