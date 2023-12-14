#pragma once

#include "libidk/libidk.hpp"
#include "libidk/IDKgl.hpp"

namespace idk { class Camera; };


class idk::Camera
{
private:
    idk::Transform      m_transform;
    glm::mat4           m_projection;
    glm::mat4           m_view;
    glm::mat4           m_rotation = glm::mat4(1.0f);

    glm::vec3           _right;
    glm::vec3           _front;
    glm::vec3           _up;

    glm::vec3           m_default_pos;
    glm::vec4           _default_right;
    glm::vec4           _default_front;
    glm::vec4           _default_up;

    float               _fov;
    float               _near;
    float               _far;

    bool                _ylock  = false;
    bool                _noroll = false;


public:

    glm::vec4           m_bloom_gamma = glm::vec4(0.02f, 2.2f, 165.0f, 1.0f);
    glm::vec4           m_exposure    = glm::vec4(1.0f,  0.0f, 10.0f, 1.0f);
    glm::vec4           m_kmhs        = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

    glm::vec2           aberration[3];

    glm::vec2           m_r_abr = glm::vec2(0.0f);
    glm::vec2           m_g_abr = glm::vec2(0.0f);
    glm::vec2           m_b_abr = glm::vec2(0.0f);
    float               m_abr_str = 0.0f;
    glm::vec3           m_offset = glm::vec3(0.0f);

                        Camera(float fov, float near, float far);
                        Camera();

    const glm::vec3     position()       { return m_transform.position(); };
    const glm::vec3     renderPosition() { return glm::inverse(view())[3]; };

    glm::mat4 &         projection()    { return m_projection; };
    glm::mat4           view();

    void                setOffset(const glm::vec3 &v);
    void                addOffset(const glm::vec3 &v);
    const glm::vec3 &   getOffset() { return m_offset; };

    void                translate(glm::vec3 v);
    void                elevation(float f);

    void                pitch(float f);
    void                roll(float f);
    void                yaw(float f);

    glm::vec3           front();
    glm::vec3           right();

    void                aspect(float width, float height);
    void                ylock(bool lock)  { _ylock = lock;  };
    void                noroll(bool lock) { _noroll = lock; };

    float               nearPlane() const { return _near; };
    float               farPlane()  const { return _far;  };

};

