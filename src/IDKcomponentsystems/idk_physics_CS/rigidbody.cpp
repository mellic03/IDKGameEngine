#include "rigidbody.h"





idk_physics::BoxCollider
idk_physics::createBoxCollider( float mass, glm::vec3 pos, glm::vec3 size )
{
    BoxCollider box;

    box.shape = new btBoxShape(btVector3(size.x, size.y, size.z));
    box.motionstate = new btDefaultMotionState(b3::translate(btVector3(pos.x, pos.y, pos.z)));

    btVector3 local_inertia = {0, 0, 0};
    box.shape->calculateLocalInertia(mass, local_inertia);

    box.rigidbody = new btRigidBody(mass, box.motionstate, box.shape, local_inertia);

    return box;
};


idk_physics::BoxCollider
idk_physics::createBoxCollider(  BoxColliderConfig config  )
{
    BoxCollider box;

    box.shape = new btBoxShape(btVector3(config.size.x, config.size.y, config.size.z));

    box.motionstate = new btDefaultMotionState(b3::translate(btVector3(config.pos.x, config.pos.y, config.pos.z)));

    btVector3 local_inertia = {0, 0, 0};
    box.shape->calculateLocalInertia(config.mass, local_inertia);

    box.rigidbody = new btRigidBody(config.mass, box.motionstate, box.shape, local_inertia);
    box.rigidbody->setFriction(config.friction);

    return box;
};

