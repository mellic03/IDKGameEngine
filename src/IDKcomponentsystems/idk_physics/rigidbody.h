#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>


namespace idk_physics
{
    struct BoxCollider;
    BoxCollider createBoxCollider( float mass, glm::vec3 pos, glm::vec3 size );
};


struct idk_physics::BoxCollider
{
    btBoxShape *                shape;
    btDefaultMotionState *      motionstate;
    btRigidBody *               rigidbody;

};

