#pragma once

#include "../IDK_builtin_CS_common.h"
#include "world.h"
#include "rigidbody.h"


class Physics_CS: public idk::ComponentSystem
{
private:
    idk_physics::World                      _physicsworld;

    std::vector<bool>                       _has_rigidbody;
    std::vector<btRigidBody *>              _rigidbodies;
    std::vector<bool>                       _has_mesh;
    std::vector<btRigidBody *>              _meshbodies;

    idk::Engine *   _engineptr;

public:
    void        init( idk::Engine & );
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );

    void        onAssignment( int obj_id, idk::Engine & );
    void        onGameObjectCreation( int obj_id, idk::Engine & );
    void        onGameObjectDeletion( int obj_id, idk::Engine & );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    void        giveBoxCollider( int obj_id, float mass, glm::vec3 size );
    void        giveBoxCollider( int obj_id, float mass );
    void        giveBoxCollider( int obj_id, idk_physics::BoxColliderConfig &config );
    void        giveMeshCollider( int obj_id, std::vector<idk::Vertex> & );

    bool            hasRigidBody( int obj_id )   { return _has_rigidbody[obj_id]; };
    btRigidBody *   getBtRigidBody( int obj_id ) { return _rigidbodies[obj_id]; };
};
