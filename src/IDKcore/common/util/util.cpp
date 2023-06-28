#include "util.h"


#define SQ(a) ((a)*(a))

float
idk::distanceSq(glm::vec3 &a, glm::vec3 &b)
{
    return SQ(a.x-b.x) + SQ(a.y-b.y) + SQ(a.z-b.z);
}

#undef SQ
