#pragma once

#include "datastructures_decl.h"


class idk::Transform
{
private:
    glm::vec3           m_front      = glm::vec3( 0.0f,  0.0f, -1.0f );
    glm::vec3           m_right      = glm::vec3( 1.0f,  0.0f,  0.0f );
    glm::vec3           m_up         = glm::vec3( 0.0f,  1.0f,  0.0f );
    glm::vec3           m_scale      = glm::vec3( 1.0f );
    glm::mat4           m_model_mat  = glm::mat4( 1.0f );

public:
                        Transform();
                        Transform( glm::mat4 m );
    
    glm::vec3           position();
    void                position( glm::vec3 );

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


