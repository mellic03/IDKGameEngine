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
    glm::vec3 front = glm::normalize(dir);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(dir, up));

    _view = glm::lookAt(
        pos,
        pos + front,
        up
    ); 
    pos = glm::vec3(0.0f, 1.0f, 3.0f);

    _projection = glm::perspective(glm::radians(_fov), 1.0f, _near, _far);
}


