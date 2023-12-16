#pragma once

#include <libidk/IDKgl.hpp>


namespace idk { class glDepthCascade; };



class idk::glDepthCascade: public idk::glFramebuffer
{
private:
    glm::vec4              m_cascade_depths;
    std::vector<glm::mat4> m_cascade_matrices;

    static const std::vector<glm::vec4> &get_corners( const glm::mat4 &P,
                                                      const glm::mat4 &V );

    static const glm::vec3 &get_center( const std::vector<glm::vec4> & );

    static const glm::mat4 &get_view( const glm::vec3 &L, const std::vector<glm::vec4> &corners );

    static const glm::mat4 &get_projection( const float texture_width,
                                            const glm::vec3 &L,
                                            const glm::mat4 &V,
                                            const std::vector<glm::vec4> &corners );


public:
    const static uint32_t NUM_CASCADES = 4;

    void        reset( int w, int h );
    void        setCascadeDepths(float a, float b, float c, float d );
    glm::vec4   getCascadeDepths( float cam_far ) { return cam_far * m_cascade_depths; };

    void        setOutputAttachment( GLint idx );
    GLuint      getTextureArray();


    const std::vector<glm::mat4> &computeCascadeMatrices( float fov,  float aspect,
                                                          float near, float far,
                                                          const glm::mat4 &cam_view,
                                                          const glm::vec3 &light_dir );

    const std::vector<glm::mat4> &getCascadeMatrices() const { return m_cascade_matrices; };

};
