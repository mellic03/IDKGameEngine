#include "rigidbody.h"




btTransform translate(btVector3 const & v)
{
	btTransform T;  // uninitialized by default
	T.setIdentity();
	T.setOrigin(v);
	return T;
}


idk_physics::BoxCollider
idk_physics::createBoxCollider( float mass, glm::vec3 pos, glm::vec3 size )
{
    BoxCollider box;

    box.shape = new btBoxShape(btVector3(size.x, size.y, size.z));
    box.motionstate = new btDefaultMotionState(translate(btVector3(pos.x, pos.y, pos.z)));

    btVector3 local_inertia = {0, 0, 0};
    box.shape->calculateLocalInertia(mass, local_inertia);

    box.rigidbody = new btRigidBody(mass, box.motionstate, box.shape, local_inertia);

    return box;
};

