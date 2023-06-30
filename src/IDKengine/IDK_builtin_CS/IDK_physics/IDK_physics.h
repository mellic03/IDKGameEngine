#pragma once

#include "../IDK_builtin_CS_common.h"
#include "world.h"
#include "rigidbody.h"


class Physics_CS: public idk::ComponentSystem
{
private:
    GLuint                          _default_shader;
    idk_physics::World              _physicsworld;
    std::vector<idk::Transform>     _transforms;

public:
    void        init( idk::Engine & );
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );


    void        onAssignment( int obj_id, idk::Engine & );
    void        onGameObjectCreation( int obj_id );
    void        onGameObjectDeletion( int obj_id );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id );


    glm::vec3 & getVelocity( int obj_id );

};
