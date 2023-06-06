#pragma once

#include "IDKgraphics_common/IDKgraphics_common.h"
#include "IDK_glShader.h"
#include "model.h"

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
    struct                  ScreenBuffer;

                            glInterface();

    void                    genScreenBuffer(GLuint &FBO, GLuint &RBO, idk::vector<GLuint> &textures);
    void                    genScreenBuffer(glInterface::ScreenBuffer &screenbuffer);
    
    GLuint                  compileShaderProgram(std::string root, std::string vs, std::string fs);
    void                    bindShaderProgram(GLuint shader_id);

    GLuint                  loadTexture(std::string filepath);

    GLuint                  pop_glTextureUnitID();
    void                    free_glTextureUnitIDs();

                            /** All subsequent draw calls will be drawn to this framebuffer */
    void                    bind_framebuffer(GLuint framebuffer_id);
    void                    draw_model(idk::Model &, idk::transform &);
    void                    draw_model(idk::Model &, idk::transform &, idk::glShader &);

    void                    setint(const char *, int);
    void                    setfloat(const char *, float);
    void                    setvec2(const char *, glm::vec2 &);
    void                    setvec3(const char *, glm::vec3 &);
    void                    setvec4(const char *, glm::vec4 &);
    void                    setmat3(const char *, glm::mat3 &);
    void                    setmat4(const char *, glm::mat4 &);


    void                    setvec2(const char *, float *);
    void                    setvec3(const char *, float *);
    void                    setvec4(const char *, float *);
    void                    setmat3(const char *, float *);
    void                    setmat4(const char *, float *);

};



struct idk::glInterface::ScreenBuffer
{
    GLuint FBO, RBO;
    idk::vector<GLuint> textures;
    ScreenBuffer(size_t num_textures): textures(num_textures) {  };
};
