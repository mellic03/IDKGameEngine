#pragma once

#include "datastructures_decl.h"


class idk::Transform
{
private:
    glm::vec3           _front      = glm::vec3( 0.0f,  0.0f, -1.0f );
    glm::vec3           _right      = glm::vec3( 1.0f,  0.0f,  0.0f );
    glm::vec3           _up         = glm::vec3( 0.0f,  1.0f,  0.0f );
    glm::vec3           _scale      = glm::vec3( 1.0f );
    glm::mat4           _model_mat  = glm::mat4( 1.0f );

public:
                        Transform();
                        Transform( glm::mat4 m );
    
    glm::vec3           position()      { return glm::vec3(_model_mat[3]); };
    glm::vec3           scale();
    glm::vec3           rotation();


    void                translate( glm::vec3 );
    void                localTranslate( glm::vec3 );
    void                pitch( float );
    void                roll( float );
    void                yaw( float );

    void                scale( glm::vec3 );
    void                rotateX( float x );
    void                rotateY( float x );
    void                rotateZ( float x );
    void                rotate( glm::vec3 v );

    glm::mat4           modelMatrix();

};


