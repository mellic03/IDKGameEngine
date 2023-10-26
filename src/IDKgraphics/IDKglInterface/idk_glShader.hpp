#pragma once

#include "IDKcommon/IDKcommon.h"
#include "idk_glBindings.hpp"

namespace idk { class glShader; };



class idk::glShader
{
private:
    struct Loc { GLint value = -1; };
    struct Def { std::string value = "NONE"; };

    std::string                 m_version;
    std::string                 m_vert_src;
    std::string                 m_frag_src;

    std::set<std::string>       m_uniforms;
    std::map<std::string, Loc>  m_locations;
    std::map<std::string, Def>  m_definitions;
    GLuint                      m_texture_unit;
    GLuint                      m_program_id;

    std::string parse_shader_include( std::string root, std::string includeline );
    std::string parse_shader_source( std::string root, std::string glsl );


public:

    void        load( std::string root, std::string vert, std::string frag );
    void        loadc( std::string root, std::string vert, std::string frag );
    bool        setDefinition( std::string name, std::string value );
    GLuint      compile();
    void        bind();
    void        unbind();


    GLint       uniformLoc    ( const std::string &name );

    void        set_int       ( std::string, int       );
    void        set_float     ( std::string, float     );
    void        set_vec2      ( std::string, glm::vec2 );
    void        set_vec3      ( std::string, glm::vec3 );
    void        set_vec4      ( std::string, glm::vec4 );
    void        set_mat3      ( std::string, glm::mat3 );
    void        set_mat4      ( std::string, glm::mat4 );
    void        set_sampler2D ( std::string, GLuint    );
    void        set_sampler3D ( std::string, GLuint    );
};

