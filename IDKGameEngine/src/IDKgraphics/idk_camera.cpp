#include "idk_camera.hpp"

idk::Camera::Camera(float fov, float near, float far):
m_projection(1.0f), m_view(1.0f),
_fov(fov), _near(near), _far(far)
{

}


idk::Camera::Camera():
Camera(95.0f, 0.1f, 200.0f)
{
    glm::vec3 pos = m_transform.position();

    pos    = glm::vec3( 0.0f,  0.0f,  0.0f );
    _front = glm::vec3( 0.0f,  0.0f, -1.0f );
    _right = glm::vec3( 1.0f,  0.0f,  0.0f );
    _up    = glm::vec3( 0.0f,  1.0f,  0.0f );

    m_default_pos  = pos;
    _default_front = glm::vec4(_front, 0.0f);
    _default_right = glm::vec4(_right, 0.0f);
    _default_up    = glm::vec4(_up,    0.0f);

    m_view = glm::lookAt(
        pos,
        pos + _front,
        _up
    ); 

    m_projection = glm::perspective(glm::radians(_fov), 1.0f, _near, _far);
}


void
idk::Camera::aspect(float w, float h)
{
    m_projection = glm::perspective(glm::radians(_fov), w/h, _near, _far);
}



void
idk::Camera::setOffset(const glm::vec3 &v)
{
    m_offset = v;

    m_default_pos = v;
    m_default_pos.y = 0.0f;

    m_view = glm::lookAt(
        m_default_pos,
        m_default_pos + _front,
        _up
    );
}



void
idk::Camera::addOffset(const glm::vec3 &v)
{
    m_offset += v;

    m_default_pos += v;
    m_default_pos.y = 0.0f;

    m_view = glm::lookAt(
        m_default_pos,
        m_default_pos + _front,
        _up
    );
}


void
idk::Camera::translate(glm::vec3 v)
{
    m_transform.translate(v);
}


void
idk::Camera::elevation(float f)
{
    m_transform.translate(glm::vec3(0.0f, f, 0.0f));
}


void
idk::Camera::pitch(float f)
{
    m_transform.pitch(f);
}


void
idk::Camera::roll(float f)
{
    // m_transform.roll(f);

    m_view = glm::rotate(m_view, f, _front);

    if (_noroll == false)
    {
        _right = glm::inverse(m_view) * _default_right;
        _up = glm::inverse(m_view) * _default_up;
    }

}


void
idk::Camera::yaw(float f)
{
    m_transform.yaw(f);
}


glm::vec3
idk::Camera::front()
{
    return m_transform.front();
}



glm::vec3
idk::Camera::right()
{
    return m_transform.right();
}



glm::mat4
idk::Camera::view()
{
    // inverse model matrix needed because world needs
    // to be transformed the opposite way the camera has moved.
    return m_view * glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, m_offset.y, 0.0f)) * m_transform.modelMatrix());
}

