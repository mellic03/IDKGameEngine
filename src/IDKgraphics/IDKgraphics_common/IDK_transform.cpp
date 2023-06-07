#include "IDK_transform.h"

#include <iostream>


idk::transform::transform():
_position(0.0f), _velocity(0.0f), _scale(1.0f)
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


glm::mat4
idk::transform::modelMatrix()
{
    glm::mat4 model_mat(1.0f);
    
    model_mat = glm::translate(model_mat, _position);
    model_mat = glm::scale(model_mat, _scale);

    return model_mat;
}
