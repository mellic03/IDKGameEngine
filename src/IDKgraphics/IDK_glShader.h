#pragma once

#include "IDKgraphics_common/IDKgraphics_common.h"

class idk::glShader
{
private:

    idk::vector<idk::pair<std::string, int>>            _uniforms_int;
    idk::vector<idk::pair<std::string, float>>          _uniforms_float;
    idk::vector<idk::pair<std::string, glm::vec2>>      _uniforms_vec2;
    idk::vector<idk::pair<std::string, glm::vec3>>      _uniforms_vec3;
    idk::vector<idk::pair<std::string, glm::vec4>>      _uniforms_vec4;
    idk::vector<idk::pair<std::string, glm::mat3>>      _uniforms_mat3;
    idk::vector<idk::pair<std::string, glm::mat4>>      _uniforms_mat4;

public:
            glShader()  { clear(); };

    void    clear();

    void    setint(std::string name, int i)             { _uniforms_int.push({name, i});   };
    void    setfloat(std::string name, float f)         { _uniforms_float.push({name, f}); };
    void    setvec2(std::string name, glm::vec2 v)      { _uniforms_vec2.push({name, v});  };
    void    setvec3(std::string name, glm::vec3 v)      { _uniforms_vec3.push({name, v});  };
    void    setvec4(std::string name, glm::vec4 v)      { _uniforms_vec4.push({name, v});  };
    void    setmat3(std::string name, glm::mat3 m)      { _uniforms_mat3.push({name, m});  };
    void    setmat4(std::string name, glm::mat4 m)      { _uniforms_mat4.push({name, m});  };


    idk::vector<idk::pair<std::string, int>> &          getUniforms_int()   { return _uniforms_int;   };
    idk::vector<idk::pair<std::string, float>> &        getUniforms_float() { return _uniforms_float; };
    idk::vector<idk::pair<std::string, glm::vec2>> &    getUniforms_vec2()  { return _uniforms_vec2;  };
    idk::vector<idk::pair<std::string, glm::vec3>> &    getUniforms_vec3()  { return _uniforms_vec3;  };
    idk::vector<idk::pair<std::string, glm::vec4>> &    getUniforms_vec4()  { return _uniforms_vec4;  };
    idk::vector<idk::pair<std::string, glm::mat3>> &    getUniforms_mat3()  { return _uniforms_mat3;  };
    idk::vector<idk::pair<std::string, glm::mat4>> &    getUniforms_mat4()  { return _uniforms_mat4;  };
};




