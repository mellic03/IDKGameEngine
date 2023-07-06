#pragma once

#include "datastructures_decl.h"


class idk::Transform
{
private:
    glm::vec3           _position;
    glm::vec3           _scale;
    glm::quat           _orientation;
    glm::mat4           _model_mat;

public:
                        Transform();
                        Transform( glm::mat4 m );
    
    glm::vec3           position()      { return glm::vec3(_model_mat[3]); };
    glm::vec3           scale();
    glm::vec3           rotation();
    glm::quat &         orientation()   { return _orientation;  };


    void                translate( glm::vec3 );
    void                scale( glm::vec3 );
    void                rotateX( float x );
    void                rotateY( float x );
    void                rotateZ( float x );
    void                rotate( glm::vec3 v );

    glm::mat4           modelMatrix();

};


