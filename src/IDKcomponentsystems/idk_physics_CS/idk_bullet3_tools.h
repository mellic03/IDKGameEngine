#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>


namespace bullet3_tools
{
    btTransform translate(btVector3 const & v);

    btVector3   btVec3_cast(glm::vec3 v);
    glm::vec3   glmvec3_cast(btVector3 v);
};
