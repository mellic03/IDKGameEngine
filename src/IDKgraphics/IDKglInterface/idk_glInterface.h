#pragma once

#include <string>
#include "IDKcommon/IDKcommon.h"

#include "idk_glUniforms.h"
#include "idk_glFramebuffer.h"


#define IDK_GLINTERFACE_MAX_TEXTUREi (GL_TEXTURE0 + 32)

namespace idk::glInterface
{
    void                    init();

    GLuint                  loadTexture( std::string filepath, int w, int h, uint8_t* &data, bool srgb = false );

    void                    genIdkFramebuffer(int width, int height, GLuint &FBO, GLuint &RBO, idk::vector<GLuint> &textures);
    void                    genIdkFramebuffer(int width, int height, glFramebuffer &);
    void                    bindIdkFramebuffer(int width, int height, glFramebuffer &);
    
    GLuint                  compileShaderProgram(std::string root, std::string vs, std::string fs);

    GLuint                  popTextureUnitID();
    void                    freeTextureUnitIDs();

    void                    setUniform_texture(std::string name, GLuint texture_id);

    void                    useProgram(GLuint shader_id);

    void                    setint(std::string, int);
    void                    setfloat(std::string, float);
    void                    setvec2(std::string, glm::vec2);
    void                    setvec3(std::string, glm::vec3);
    void                    setvec4(std::string, glm::vec4);
    void                    setmat3(std::string, glm::mat3);
    void                    setmat4(std::string, glm::mat4);

    void                    setvec2(std::string, const float *);
    void                    setvec3(std::string, float *);
    void                    setvec4(std::string, float *);
    void                    setmat3(std::string, float *);
    void                    setmat4(std::string, float *);

};
