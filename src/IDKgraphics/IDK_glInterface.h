#pragma once

#include "../IDKcore/IDKcore.h"

#define IDK_GLINTERFACE_MAX_TEXTUREi (GL_TEXTURE0 + 32)

class idk::glInterface
{
private:
    GLuint                  _active_shader_id;
    idk::stack<GLuint>      _available_glTextureUnits;
    idk::stack<GLuint>      _unavailable_glTextureUnits;

    bool                    _line_has_include(std::string &line);
    std::string             _parse_shader_include(std::string root, std::string line);
    std::string             _parse_shader_source(std::string root, std::string glsl);


public:
                            glInterface();

    GLuint                  compileShaderProgram(std::string root, std::string vs, std::string fs);
    void                    bindShaderProgram(GLuint shader_id);

    GLuint                  loadTexture(std::string filepath);

    GLuint                  pop_glTextureUnitID();
    void                    free_glTextureUnitIDs();

    void                    setint(const char *, int);
    void                    setfloat(const char *, float);
    void                    setvec2(const char *, idk::vec2 &);
    void                    setvec3(const char *, idk::vec3 &);
    void                    setvec4(const char *, idk::vec4 &);
    void                    setmat3(const char *, idk::mat3 &);
    void                    setmat4(const char *, idk::mat4 &);
};
