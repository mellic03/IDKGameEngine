#pragma once

#include <string>
#include <sstream>
#include <fstream>

#include "../IDKcore/IDKcore.h"


class IDK::Shader
{
private:
    std::string _vert_src, _frag_src;
    GLuint _program_id;

public:
    Shader() {  };
    Shader(std::string dir, std::string vs, std::string fs);

    void setint(const char *, int);
    void setfloat(const char *, float);

    void setvec2(const char *, IDK::vec2 &);
    void setvec3(const char *, IDK::vec3 &);
    void setvec4(const char *, IDK::vec4 &);
    void setmat3(const char *, IDK::mat3 &);
    void setmat4(const char *, IDK::mat4 &);

    GLuint id() const { return _program_id; };
};

