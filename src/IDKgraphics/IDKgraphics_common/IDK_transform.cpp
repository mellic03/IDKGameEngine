#include "IDK_transform.h"

#include <iostream>


idk::transform::transform():
_position(0.0f), _velocity(0.0f), _scale(1.0f),
_orientation(glm::vec3(0.0f)), _model_mat(1.0f)
{

}


void
idk::transform::translate(glm::vec3 t)
{
    _position += t;
}


void
idk::transform::scale(glm::vec3 s)
{
    _scale *= s;
}


void
idk::transform::rotateX(float x)
{
    glm::quat rotX = glm::quat(glm::vec3(x, 0.0f, 0.0f));
    _orientation = rotX * _orientation;
}

void
idk::transform::rotateY(float y)
{
    glm::quat rotY = glm::quat(glm::vec3(0.0f, y, 0.0f));
    _orientation = rotY * _orientation;
}

void
idk::transform::rotateZ(float z)
{
    glm::quat rotZ = glm::quat(glm::vec3(0.0f, 0.0f, z));
    _orientation = rotZ * _orientation;
}


glm::mat4
idk::transform::modelMatrix()
{
    glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), _position) * glm::mat4_cast(_orientation);
    model_mat = glm::scale(model_mat, _scale);

    return model_mat;
}

