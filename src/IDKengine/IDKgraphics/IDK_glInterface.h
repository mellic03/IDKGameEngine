#pragma once

#include "IDKcore.h"
#include "IDK_glUniforms.h"
#include "IDK_glFramebuffer.h"
#include "IDK_model.h"

#define IDK_GLINTERFACE_MAX_TEXTUREi (GL_TEXTURE0 + 32)

class idk::glInterface
{
private:
    GLuint                  _active_shader_id;
    idk::vector<GLuint>     _available_glTextureUnits;
    idk::vector<GLuint>     _unavailable_glTextureUnits;

    Allocator<Material>                 _material_allocator;
    Allocator<GLuint>                   _shader_allocator;
    std::unordered_map<std::string, GLuint> _textures;


    bool                    _line_has_include(std::string &line);
    std::string             _parse_shader_include(std::string root, std::string line);
    std::string             _parse_shader_source(std::string root, std::string glsl);

    GLuint                  _load_texture(std::string filepath, bool srgb);

    GLuint                  pop_glTextureUnitID();
    void                    free_glTextureUnitIDs();

public:
                            glInterface();

    void                    genFramebuffer(int width, int height, GLuint &FBO, GLuint &RBO, idk::vector<GLuint> &textures);
    void                    genFramebuffer(int width, int height, glFramebuffer &);
    void                    bindFramebuffer(int width, int height, glFramebuffer &);
    
    GLuint                  compileShaderProgram(std::string root, std::string vs, std::string fs);
    void                    bindShaderProgram(GLuint shader_id);

    void                    loadTexture(std::string filepath);

    void                    setUniform_texture(std::string name, GLuint texture_id);
    void                    bindMaterial(idk::Material &material);

    Allocator<Material> &   materials() { return _material_allocator; };
    std::unordered_map<std::string, GLuint> &textures() { return _textures; };

    void                    drawModel( idk::Model &, idk::Transform & );
    void                    drawModel( idk::Model &, idk::Transform &, idk::glUniforms & );
    void                    drawVertices( GLuint VAO, int num_vertices );

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


    // State change ---------------------------------------------------------------------------

    void                    enable(GLenum cap)      { GLCALL( glEnable(cap);  ) };
    void                    disable(GLenum cap)     { GLCALL( glDisable(cap); ) };
    void                    enable()  {  };
    void                    disable() {  };

    template <typename... GLenums> void     enable( GLenum, GLenums... );
    template <typename... GLenums> void     disable( GLenum, GLenums... );
    
    
    // ----------------------------------------------------------------------------------------

};



template <typename... GLenums>
void
idk::glInterface::enable( GLenum cap, GLenums... rest )
{
    enable(cap);
    enable(rest...);
}


template <typename... GLenums>
void
idk::glInterface::disable( GLenum cap, GLenums... rest )
{
    disable(cap);
    disable(rest...);
}
