#include "IDK_camera.h"

idk::Camera::Camera(float fov, float near, float far):
_projection(1.0f), _view(1.0f),
_fov(fov), _near(near), _far(far)
{

}


idk::Camera::Camera():
Camera(90.0f, 0.1f, 200.0f)
{
    glm::vec3 &pos = _transform.position();
   
    pos    = glm::vec3( 0.0f,  0.0f,  0.0f );
    _front = glm::vec3( 0.0f,  0.0f,  -1.0f );
    _right = glm::vec3( 1.0f,  0.0f,  0.0f );
    _up    = glm::vec3( 0.0f,  1.0f,  0.0f );

    _view = glm::lookAt(
        pos,
        pos + _front,
        _up
    ); 

    _projection = glm::perspective(glm::radians(_fov), 1.0f, _near, _far);
}


void
idk::Camera::aspect(float w, float h)
{
    _projection = glm::perspective(glm::radians(_fov), w/h, _near, _far);
}


void
idk::Camera::translate(glm::vec3 v)
{
    v = glm::inverse(glm::mat3(_view)) * v;

    if (_ylock)
    {
        v.y = 0.0f;
    }

    _transform.translate(v);
}


void
idk::Camera::elevation(float f)
{
    _transform.position().y += f;
}


void
idk::Camera::pitch(float f)
{
    _view = glm::rotate(_view, f, _right);
}


void
idk::Camera::yaw(float f)
{
    _view = glm::rotate(_view, f, glm::vec3(0.0f, 1.0f, 0.0f));
    _right = glm::inverse(_view) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    _front = glm::inverse(_view) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
}


glm::mat4
idk::Camera::view()
{
    // _pitch = idk::clamp(_pitch, -1.0f, 1.0f);

    // glm::mat4 rot_view = glm::rotate(_view, _yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    // glm::vec4 right(_right, 0.0f);
    // right = glm::inverse(rot_view) * right;
    // rot_view = glm::rotate(rot_view, _pitch, glm::vec3(right));

    return _view * _transform.modelMatrix();
}

