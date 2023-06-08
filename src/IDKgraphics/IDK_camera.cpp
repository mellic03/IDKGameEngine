#include "IDK_camera.h"

idk::Camera::Camera(float fov, float near, float far):
_projection(1.0f), _view(1.0f),
_fov(fov), _near(near), _far(far)
{

}


idk::Camera::Camera():
Camera(85.0f, 0.1f, 200.0f)
{
    glm::vec3 &pos = _transform.position();

    glm::vec3 target = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 dir = glm::normalize(target - pos);
    _front = glm::normalize(dir);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    _right = glm::normalize(glm::cross(dir, up));

    _view = glm::lookAt(
        pos,
        pos + _front,
        up
    ); 

    _projection = glm::perspective(glm::radians(_fov), 1.0f, _near, _far);
}


void
idk::Camera::pitch(float f)
{
    _transform.rotateX(f);
}


void
idk::Camera::yaw(float f)
{
    _transform.rotateY(f);
}



glm::mat4
idk::Camera::view()
{
    return _view * _transform.modelMatrix();
}

