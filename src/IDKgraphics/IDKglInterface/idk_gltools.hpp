#pragma once

#include <string>
#include "IDKcommon/IDKcommon.h"

#include "idk_glFramebuffer.hpp"
#include "idk_glBindings.hpp"


#define IDK_GLINTERFACE_MAX_TEXTUREi (GL_TEXTURE0 + 32)


namespace idk::gltools
{
    void                    init();

    GLuint                  loadTexture( int w, int h, uint32_t *data, bool srgb = false );

    GLuint                  compileProgram(std::string root, std::string vs, std::string fs);
    void                    useProgram(GLuint shader_id);

    GLuint                  popTextureUnitID();
    void                    freeTextureUnitIDs();

    void                    setUniform_texture(std::string name, GLuint texture_id);
    void                    setUniform_texture3D(std::string name, GLuint texture_id);
    void                    setUniform_int(std::string, int);
    void                    setUniform_float(std::string, float);
    void                    setUniform_vec2(std::string, glm::vec2);
    void                    setUniform_vec3(std::string, glm::vec3);
    void                    setUniform_vec4(std::string, glm::vec4);
    void                    setUniform_mat3(std::string, glm::mat3);
    void                    setUniform_mat4(std::string, glm::mat4);

    void                    setUniform_texture(GLint loc, GLuint texture_id);
    void                    setUniform_int(GLint loc, int);
    void                    setUniform_float(GLint loc, float);
    void                    setUniform_vec2(GLint loc, glm::vec2);
    void                    setUniform_vec3(GLint loc, glm::vec3);
    void                    setUniform_vec4(GLint loc, glm::vec4);
    void                    setUniform_mat3(GLint loc, glm::mat3);
    void                    setUniform_mat4(GLint loc, glm::mat4);

};

