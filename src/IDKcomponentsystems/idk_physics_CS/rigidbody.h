#pragma once

#include "idk_bullet3_tools.h"


namespace idk_physics
{
    struct BoxCollider;
    struct BoxColliderConfig;

    BoxCollider createBoxCollider( float mass, glm::vec3 pos, glm::vec3 size );
    BoxCollider createBoxCollider( BoxColliderConfig config );
};


struct idk_physics::BoxCollider
{
    int                         obj_id;
    btBoxShape *                shape;
    btDefaultMotionState *      motionstate;
    btRigidBody *               rigidbody;

};


struct idk_physics::BoxColliderConfig
{
    float mass;
    float friction;
    glm::vec3 pos, size;

};

