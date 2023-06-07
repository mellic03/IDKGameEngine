#pragma once

#include "../../IDKcore/IDKcore.h"


class idk::transform
{
private:
    glm::vec3           _position;
    glm::vec3           _velocity;
    glm::vec3           _scale;
    glm::quat           _orientation;
    glm::mat4           _model_mat;

public:
                        transform();
    
    glm::vec3 &         position()  { return _position; };
    glm::vec3 &         velocity()  { return _velocity; };

    void                translate(glm::vec3);
    void                scale(glm::vec3);
    void                rotateX(float x);
    void                rotateY(float x);
    void                rotateZ(float x);

    glm::mat4           modelMatrix();

};


