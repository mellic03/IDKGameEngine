#include "idk_bullet3_tools.h"

btTransform
b3::translate(btVector3 const & v)
{
    btTransform T;  // uninitialized by default
    T.setIdentity();
    T.setOrigin(v);
    return T;
}


btVector3
b3::btVec3_cast(glm::vec3 v)
{
    return btVector3(v.x, v.y, v.z);
}


glm::vec3
b3::glmvec3_cast(btVector3 v)
{
    return glm::vec3(v.getX(), v.getY(), v.getZ());
}


void
b3::applyCentralForce(btRigidBody *rigidbody, glm::vec3 force)
{
    rigidbody->applyCentralForce(b3::btVec3_cast(force));
}

