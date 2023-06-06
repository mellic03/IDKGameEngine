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


GLuint
idk::glInterface::loadTexture(std::string filepath)
{
    GLuint texture_id;

    unsigned char *data;
    std::ifstream instream(filepath);

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

    return texture_id;
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
    _active_shader_id = shader_id;
    GLCALL( glUseProgram(shader_id); )
}


void
idk::glInterface::setint(const char *name, int i)
{
    GLuint loc = glGetUniformLocation(_active_shader_id, name);
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
idk::glInterface::setvec2(const char *name, idk::vec2 &v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform2fv(loc, 1, idk::value_ptr(v));
    )
}

void
idk::glInterface::setvec3(const char *name, idk::vec3 &v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform3fv(loc, 1, idk::value_ptr(v));
    )
}

void
idk::glInterface::setvec4(const char *name, idk::vec4 &v)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniform4fv(loc, 1, idk::value_ptr(v));
    )
}

void
idk::glInterface::setmat3(const char *name, idk::mat3 &m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniformMatrix3fv(loc, 1, GL_FALSE, idk::value_ptr(m));
    )
}

void
idk::glInterface::setmat4(const char *name, idk::mat4 &m)
{
    GLCALL(
        GLuint loc = glGetUniformLocation(_active_shader_id, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, idk::value_ptr(m));
    )
}
