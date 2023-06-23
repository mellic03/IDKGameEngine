#include "IDK_transform.h"

#include <iostream>


idk::Transform::Transform():
_position(0.0f), _scale(1.0f),
_orientation(glm::vec3(0.0f)), _model_mat(1.0f)
{

}


void
idk::Transform::translate(glm::vec3 t)
{
    _position += t;
}


void
idk::Transform::scale(glm::vec3 s)
{
    _scale *= s;
}


void
idk::Transform::rotateX(float x)
{
    glm::quat rotX = glm::quat(glm::vec3(x, 0.0f, 0.0f));
    _orientation = rotX * _orientation;
}

void
idk::Transform::rotateY(float y)
{
    glm::quat rotY = glm::quat(glm::vec3(0.0f, y, 0.0f));
    _orientation = rotY * _orientation;
}

void
idk::Transform::rotateZ(float z)
{
    glm::quat rotZ = glm::quat(glm::vec3(0.0f, 0.0f, z));
    _orientation = rotZ * _orientation;
}


glm::mat4
idk::Transform::modelMatrix()
{
    glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), _position) * glm::mat4_cast(_orientation);
    model_mat = glm::scale(model_mat, _scale);

    return model_mat;
}

