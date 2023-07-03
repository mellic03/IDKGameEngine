#pragma once

#include <string>
#include "IDKcommon/IDKcommon.h"

#include "idk_glUniforms.h"
#include "idk_glFramebuffer.h"


namespace idk { class glInterface; };


#define IDK_GLINTERFACE_MAX_TEXTUREi (GL_TEXTURE0 + 32)

class idk::glInterface
{
private:
    GLuint                      _active_shader_id;
    idk::vector<GLuint>         _available_glTextureUnits;
    idk::vector<GLuint>         _unavailable_glTextureUnits;

    // idk::Allocator<Material>                    _material_allocator;
    // idk::Allocator<Texture>                     _texture_allocator;
    // std::unordered_map<std::string, GLuint>     _textures;


    bool                    _line_has_include(std::string &line);
    std::string             _parse_shader_include(std::string root, std::string line);
    std::string             _parse_shader_source(std::string root, std::string glsl);


public:
                            glInterface();

    void                    genFramebuffer(int width, int height, GLuint &FBO, GLuint &RBO, idk::vector<GLuint> &textures);
    void                    genFramebuffer(int width, int height, glFramebuffer &);
    void                    bindFramebuffer(int width, int height, glFramebuffer &);
    
    GLuint                  compileShaderProgram(std::string root, std::string vs, std::string fs);
    void                    useProgram(GLuint shader_id);

    void                    loadTexture(std::string path, GLuint &gl_id, int &w, int &h, uint8_t* &data, bool srgb = false);

    GLuint                  pop_glTextureUnitID();
    void                    free_glTextureUnitIDs();

    void                    setUniform_texture(std::string name, GLuint texture_id);

    void                    drawArrays( GLenum mode, GLuint VAO, int num_vertices );

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
