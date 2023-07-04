#pragma once

#include <string>
#include "IDKcommon/IDKcommon.h"

#include "idk_glUniforms.h"
#include "idk_glFramebuffer.h"
#include "idk_glBindings.h"


#define IDK_GLINTERFACE_MAX_TEXTUREi (GL_TEXTURE0 + 32)

namespace idk::glInterface
{
    void                    init();

    GLuint                  loadTexture( std::string filepath, int w, int h, uint8_t* &data, bool srgb = false );

    void                    genIdkFramebuffer(int width, int height, GLuint &FBO, GLuint &RBO, idk::vector<GLuint> &textures);
    void                    genIdkFramebuffer(int width, int height, glFramebuffer &);
    void                    bindIdkFramebuffer(int width, int height, glFramebuffer &);
    void                    unbindIdkFramebuffer(int width, int height);

    GLuint                  compileShaderProgram(std::string root, std::string vs, std::string fs);
    void                    useProgram(GLuint shader_id);

    GLuint                  popTextureUnitID();
    void                    freeTextureUnitIDs();

    void                    setUniform_texture(std::string name, GLuint texture_id);
    void                    setUniform_int(std::string, int);
    void                    setUniform_float(std::string, float);
    void                    setUniform_vec2(std::string, glm::vec2);
    void                    setUniform_vec3(std::string, glm::vec3);
    void                    setUniform_vec4(std::string, glm::vec4);
    void                    setUniform_mat3(std::string, glm::mat3);
    void                    setUniform_mat4(std::string, glm::mat4);

};
