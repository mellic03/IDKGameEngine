#include "idk_glShader.hpp"
#include "idk_gltools.hpp"

#include <fstream>
#include <sstream>
#include <iostream>


std::vector<std::string> tokenize( std::string str, size_t num_tokens=~0 )
{
    std::vector<std::string> tokens;

    std::stringstream ss(str);
    std::string token;

    while (ss >> token && num_tokens > 0)
    {
        tokens.push_back(token);
        num_tokens -= 1;
    }

    return tokens;
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


std::string
idk::glShader::parse_shader_include( std::string root, std::string includeline )
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
        auto tokens = tokenize(line);

        if (tokens.size() > 0 && tokens[0] == "#version")
        {
            m_version = line + "\n";
            continue;
        }

        if (tokens.size() == 3 && tokens[0] == "uniform")
        {
            if (tokens[2].back() == ';')
                tokens[2].pop_back();

            m_uniforms.emplace(tokens[2]);
        }

        else if (tokens.size() > 0 && tokens[0] == "#define")
        {
            m_definitions[tokens[1]].value = line.substr(tokens[0].length() + tokens[1].length() + 1);
            continue;
        }


        if (_line_has_include(line))
            src += parse_shader_include(root, line);
        else
            src += line + '\n';
    }
    return src;
}


std::string
idk::glShader::parse_shader_source( std::string root, std::stringstream source )
{
    std::string src;
    std::string line;

    while (getline(source, line))
    {
        auto tokens = tokenize(line);

        if (tokens.size() > 0 && tokens[0] == "#version")
        {
            m_version = line + "\n";
            continue;
        }

        if (tokens.size() == 3 && tokens[0] == "uniform")
        {
            if (tokens[2].back() == ';')
                tokens[2].pop_back();

            m_uniforms.emplace(tokens[2]);
        }

        else if (tokens.size() > 0 && tokens[0] == "#define")
        {
            m_definitions[tokens[1]].value = line.substr(tokens[0].length() + tokens[1].length() + 1);
            continue;
        }


        if (_line_has_include(line))
            src += parse_shader_include(root, line);
        else
            src += line + "\n";
    }
    return src;
}


GLuint
idk::glShader::compileShader( std::string name, std::string &src, GLenum type )
{
    const char *str = src.c_str();
    GLuint shader_id = glCreateShader(type);

    GLCALL( glShaderSource(shader_id, 1, &str, nullptr); )
    GLCALL( glCompileShader(shader_id); )

    int result;
    GLCALL( glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result); )

    if (result == GL_FALSE)
    {
        std::cout << src << "\n";

        int length;
        GLCALL( glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length); )
        char *message = new char[length * sizeof(char)];
        GLCALL( glGetShaderInfoLog(shader_id, length, &length, message); )

        std::cout << "Name: " << name << std::endl;

        std::cout << "Failed to compile ";
        if      (type == GL_VERTEX_SHADER)   std::cout << "vertex shader\n";
        else if (type == GL_GEOMETRY_SHADER) std::cout << "geometry shader\n";
        else if (type == GL_FRAGMENT_SHADER) std::cout << "fragment shader\n";
        std::cout << message << std::endl;
        delete[] message;

        exit(1);
    }

    return shader_id;
}


GLuint
idk::glShader::compile_vf( const std::string &defines )
{
    static std::string v = "";
    static std::string f = "";

    v = m_version + defines + m_vert_src;
    f = m_version + defines + m_frag_src;

    GLuint vert_id = compileShader("Ree", v, GL_VERTEX_SHADER);
    GLuint frag_id = compileShader("Ree", f, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    GLCALL( glAttachShader(program, vert_id); )
    GLCALL( glAttachShader(program, frag_id); )
    GLCALL( glLinkProgram(program); )

    // GLchar *str = new GLchar[1024];
    // glGetProgramInfoLog(program, 1024, nullptr, str);
    // std::cout << str << "";
    // delete[] str;

    GLCALL( glValidateProgram(program); )

    GLCALL( glDeleteShader(vert_id); )
    GLCALL( glDeleteShader(frag_id); )

    return program;
}


GLuint
idk::glShader::compile_vgf( const std::string &defines )
{
    static std::string v = "";
    static std::string g = "";
    static std::string f = "";
    v = m_version + defines + m_vert_src;
    g = m_version + defines + m_geom_src;
    f = m_version + defines + m_frag_src;

    GLuint vert_id = compileShader("Ree", v, GL_VERTEX_SHADER);
    GLuint geom_id = compileShader("Ree", g, GL_GEOMETRY_SHADER);
    GLuint frag_id = compileShader("Ree", f, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    GLCALL( glAttachShader(program, vert_id); )
    GLCALL( glAttachShader(program, geom_id); )
    GLCALL( glAttachShader(program, frag_id); )
    GLCALL( glLinkProgram(program); )

    GLCALL( glValidateProgram(program); )

    GLCALL( glDeleteShader(vert_id); )
    GLCALL( glDeleteShader(geom_id); )
    GLCALL( glDeleteShader(frag_id); )

    return program;
}


void
idk::glShader::reset()
{
    m_definitions.clear();
    m_locations.clear();
    m_uniforms.clear();

    m_vert_src.clear();
    m_geom_src.clear();
    m_frag_src.clear();
}


void
idk::glShader::loadFile( std::string root, std::string vert, std::string frag )
{
    reset();

    std::stringstream vert_buf, frag_buf;
    vert_buf << std::ifstream(root + vert).rdbuf();
    frag_buf << std::ifstream(root + frag).rdbuf();

    m_vert_src = parse_shader_source(root, std::stringstream(vert_buf.str()));
    m_frag_src = parse_shader_source(root, std::stringstream(frag_buf.str()));
}


GLuint
idk::glShader::loadFileC( std::string root, std::string vert, std::string frag )
{
    loadFile(root, vert, frag);
    return this->compile();
}


void
idk::glShader::loadFile_vgf( std::string root, std::string v, std::string g, std::string f )
{
    reset();

    std::stringstream vert_buf, geom_buf, frag_buf;
    vert_buf << std::ifstream(root + v).rdbuf();
    geom_buf << std::ifstream(root + g).rdbuf();
    frag_buf << std::ifstream(root + f).rdbuf();

    m_vert_src = parse_shader_source(root, std::stringstream(vert_buf.str()));
    m_geom_src = parse_shader_source(root, std::stringstream(geom_buf.str()));
    m_frag_src = parse_shader_source(root, std::stringstream(frag_buf.str()));
}


GLuint
idk::glShader::loadFileC_vgf( std::string root, std::string v, std::string g, std::string f )
{
    loadFile_vgf(root, v, g, f);
    return this->compile();
}



void
idk::glShader::loadString( const std::string &vert, const std::string &frag )
{
    reset();

    m_vert_src = parse_shader_source("", std::stringstream(vert));
    m_frag_src = parse_shader_source("", std::stringstream(frag));
}


GLuint
idk::glShader::loadStringC( const std::string &vert, const std::string &frag )
{
    loadString(vert, frag);
    return this->compile();
}


bool
idk::glShader::setDefinition( std::string name, std::string value )
{
    if (m_definitions[name].value == "NONE")
    {
        #ifdef IDK_DEBUG
            std::cout
                << "[idk::glShader::setDefinition] Definition \"" << name
                << "\" does not exist\n";
            exit(1);
        #endif
        return false;
    }

    m_definitions[name].value = value;

    return true;
}


GLuint
idk::glShader::compile()
{
    std::string defines = "";

    for (auto &[name, value]: m_definitions)
    {
        defines += "#define " + name + " " + value.value + "\n";
    }

    gl::deleteProgram(m_program_id);
    m_program_id = compile_vf(defines);

    for (auto &name: m_uniforms)
    {
        m_locations[name].value = gl::getUniformLocation(m_program_id, name);
    }

    return m_program_id;
}


GLint
idk::glShader::uniformLoc( const std::string &name )
{
    GLint location = m_locations[name].value;

    // If -1, uniform may have been missed during parsing.
    if (location == -1)
    {
        return gl::getUniformLocation(m_program_id, name);
    }

    // #ifdef IDK_DEBUG
    //     if (location == -1)
    //     {
    //         std::cout
    //             << "[idk::glShader::uniformLoc] Uniform \"" << name
    //             << "\" location == -1\n";
    //         exit(1);
    //     }
    // #endif

    return location;
}



void
idk::glShader::bind()
{
    #ifdef IDK_DEBUG
    if (m_program_id == 0)
    {
        std::cout << "[idk::glShader::bind] Program not initialized!\n";
        exit(1);
    }
    #endif

    gl::useProgram(m_program_id);
    m_texture_unit = GL_TEXTURE0;
}


void
idk::glShader::unbind()
{
    #ifdef IDK_DEBUG
        idk::gl::useProgram(0);
    #endif
};


void
idk::glShader::set_int( std::string name, int i )
{
    GLint loc = uniformLoc(name);
    gl::uniform1i(loc, i);
}


void
idk::glShader::set_float( std::string name, float f )
{
    GLint loc = uniformLoc(name);
    gl::uniform1f(loc, f);
}


void
idk::glShader::set_vec2( std::string name, glm::vec2 v )
{
    GLint loc = uniformLoc(name);
    gl::uniform2fv(loc, 1, glm::value_ptr(v));
}


void
idk::glShader::set_vec3( std::string name, glm::vec3 v )
{
    GLint loc = uniformLoc(name);
    gl::uniform3fv(loc, 1, glm::value_ptr(v));
}


void
idk::glShader::set_vec4( std::string name, glm::vec4 v )
{
    GLint loc = uniformLoc(name);
    gl::uniform4fv(loc, 1, glm::value_ptr(v));
}


void
idk::glShader::set_mat3( std::string name, glm::mat3 m )
{
    GLint loc = uniformLoc(name);
    gl::uniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}


void
idk::glShader::set_mat4( std::string name, glm::mat4 m )
{
    GLint loc = uniformLoc(name);
    gl::uniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}


void
idk::glShader::set_sampler2D( std::string name, GLuint texture_id )
{
    if (m_program_id == 0)
    {
        std::cout << "RUH ROH" << std::endl;
        exit(1);
    }

    #ifdef IDK_DEBUG
        if (m_texture_unit - GL_TEXTURE0 > 32)
        {
            std::cout << "[idk::glShader::set_sampler2D] m_texture_unit > GL_TEXTURE0 + 32\n";
            exit(1);
        }
    #endif

    GLint loc = uniformLoc(name);

    gl::bindTextureUnit(m_texture_unit - GL_TEXTURE0, texture_id);
    gl::uniform1i(loc, m_texture_unit - GL_TEXTURE0);

    m_texture_unit += 1;
}


void
idk::glShader::set_sampler3D( std::string name, GLuint texture_id )
{
    #ifdef IDK_DEBUG
        if (m_texture_unit - GL_TEXTURE0 > GL_TEXTURE0 + 32)
        {
            std::cout << "[idk::glShader::set_sampler3D] m_texture_unit > GL_TEXTURE0 + 32\n";
            exit(1);
        }
    #endif

    GLint loc = uniformLoc(name);

    gl::activeTexture(m_texture_unit);
    gl::bindTexture(GL_TEXTURE_3D, texture_id);
    gl::uniform1i(loc, m_texture_unit - GL_TEXTURE0);

    m_texture_unit += 1;
}



void
idk::glShader::set_samplerCube( std::string name, GLuint texture_id )
{
    if (m_program_id == 0)
    {
        std::cout << "RUH ROH" << std::endl;
        exit(1);
    }

    #ifdef IDK_DEBUG
        if (m_texture_unit - GL_TEXTURE0 > 32)
        {
            std::cout << "[idk::glShader::set_sampler2D] m_texture_unit > GL_TEXTURE0 + 32\n";
            exit(1);
        }
    #endif

    GLint loc = uniformLoc(name);

    gl::activeTexture(m_texture_unit);
    gl::bindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    gl::uniform1i(loc, m_texture_unit - GL_TEXTURE0);

    m_texture_unit += 1;
}