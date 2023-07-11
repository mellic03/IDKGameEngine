#include "idk_bullet3_tools.h"

btTransform
bullet3_tools::translate(btVector3 const & v)
{
    btTransform T;  // uninitialized by default
    T.setIdentity();
    T.setOrigin(v);
    return T;
}


btVector3
bullet3_tools::btVec3_cast(glm::vec3 v)
{
    return btVector3(v.x, v.y, v.z);
}


glm::vec3
bullet3_tools::glmvec3_cast(btVector3 v)
{
    return glm::vec3(v.getX(), v.getY(), v.getZ());
}

