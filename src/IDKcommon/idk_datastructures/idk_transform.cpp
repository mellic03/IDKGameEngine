#include "idk_transform.h"


idk::Transform::Transform():
_position(0.0f), _scale(1.0f),
_orientation(glm::vec3(0.0f)), _model_mat(1.0f)
{

}

idk::Transform::Transform( glm::mat4 m ):
_position(0.0f), _scale(1.0f),
_orientation(glm::vec3(0.0f)), _model_mat(m)
{

}


glm::vec3
idk::Transform::scale()
{
    return glm::vec3(
        _model_mat[0][0],
        _model_mat[1][1],
        _model_mat[2][2]
    );
}


void
idk::Transform::translate(glm::vec3 t)
{
    _model_mat = glm::translate(_model_mat, t);
}


void
idk::Transform::scale(glm::vec3 s)
{
    _scale = glm::vec3(_scale.x*s.x, _scale.y*s.y, _scale.z*s.z );
}


void
idk::Transform::rotateX(float x)
{
    glm::quat rotX = glm::quat(glm::vec3(x, 0.0f, 0.0f));
    _model_mat = _model_mat * glm::mat4_cast(rotX);
}

void
idk::Transform::rotateY(float y)
{
    glm::quat rotY = glm::quat(glm::vec3(0.0f, y, 0.0f));
    _model_mat = _model_mat * glm::mat4_cast(rotY);
}

void
idk::Transform::rotateZ(float z)
{
    _model_mat = glm::rotate(_model_mat, z, glm::vec3(0.0f, 0.0f, 1.0f));
//     glm::quat rotZ = glm::quat(glm::vec3(0.0f, 0.0f, z));
//     _model_mat = _model_mat * glm::mat4_cast(rotZ);
}


void
idk::Transform::rotate(glm::vec3 v)
{
    glm::quat rot = glm::quat(v);
    _model_mat = _model_mat * glm::mat4_cast(rot);
}


glm::mat4
idk::Transform::modelMatrix()
{
    // glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), _position) * glm::mat4_cast(_orientation);
    // model_mat = glm::scale(model_mat, _scale);

    return glm::scale(_model_mat, _scale);
}

