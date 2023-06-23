#pragma once

#include "../../IDKcore/IDKcore.h"


class idk::Transform
{
private:
    glm::vec3           _position;
    glm::vec3           _velocity;
    glm::vec3           _scale;
    glm::quat           _orientation;
    glm::mat4           _model_mat;

public:
                        Transform();
    
    glm::vec3 &         position()      { return _position;     };
    glm::vec3 &         velocity()      { return _velocity;     };
    glm::quat &         orientation()   { return _orientation;  };

    void                translate(glm::vec3);
    void                scale(glm::vec3);
    void                rotateX(float x);
    void                rotateY(float x);
    void                rotateZ(float x);

    glm::mat4           modelMatrix();

};


