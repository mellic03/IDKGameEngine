#pragma once

#include "../../IDKcore/IDKcore.h"


class idk::transform
{
private:
    glm::vec3 _position;
    glm::vec3 _velocity;
    glm::vec3 _scale;

public:
                        transform(): _scale(1.0f) {  };
    
    void                translate(glm::vec3);
    void                scale(glm::vec3);

    glm::mat4           modelMatrix();
    void                modelMatrixStale();

};

