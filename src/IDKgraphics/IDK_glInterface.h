#pragma once

#include "IDKgraphics_common/IDKgraphics_common.h"
#include "IDK_glUniforms.h"
#include "model.h"

#define IDK_GLINTERFACE_MAX_TEXTUREi (GL_TEXTURE0 + 32)

class idk::glInterface
{
private:
    GLuint                  _active_shader_id;
    idk::vector<GLuint>     _available_glTextureUnits;
    idk::vector<GLuint>     _unavailable_glTextureUnits;

    Allocator<Material>                 _material_allocator;
    std::unordered_map<std::string, GLuint> _textures;


    bool                    _line_has_include(std::string &line);
    std::string             _parse_shader_include(std::string root, std::string line);
    std::string             _parse_shader_source(std::string root, std::string glsl);
    
    GLuint                  pop_glTextureUnitID();
    void                    free_glTextureUnitIDs();

public:
    struct                  ScreenBuffer;

                            glInterface();

    void                    genScreenBuffer(int width, int height, GLuint &FBO, GLuint &RBO, idk::vector<GLuint> &textures);
    void                    genScreenBuffer(int width, int height, glInterface::ScreenBuffer &screenbuffer);
    void                    bindScreenbuffer(int width, int height, idk::glInterface::ScreenBuffer &screen_buffer);
    
    GLuint                  compileShaderProgram(std::string root, std::string vs, std::string fs);
    void                    bindShaderProgram(GLuint shader_id);

    void                    loadTexture(std::string filepath);

    void                    setUniform_texture(std::string name, GLuint texture_id);
    void                    bindMaterial(idk::Material &material);

    Allocator<Material> &   materials() { return _material_allocator; };
    std::unordered_map<std::string, GLuint> &textures() { return _textures; };

                            /** All subsequent draw calls will be drawn to this framebuffer */
    void                    draw_model(idk::Model &, idk::transform &);

    void                    setint(const char *, int);
    void                    setfloat(const char *, float);
    void                    setvec2(const char *, glm::vec2);
    void                    setvec3(const char *, glm::vec3);
    void                    setvec4(const char *, glm::vec4);
    void                    setmat3(const char *, glm::mat3);
    void                    setmat4(const char *, glm::mat4);

    void                    setvec2(const char *, const float *);
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
