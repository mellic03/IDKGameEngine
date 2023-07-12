#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>


namespace b3
{
    btTransform translate(btVector3 const & v);

    btVector3   btVec3_cast(glm::vec3 v);
    glm::vec3   glmvec3_cast(btVector3 v);


    void        applyCentralForce(btRigidBody *, glm::vec3);
};
