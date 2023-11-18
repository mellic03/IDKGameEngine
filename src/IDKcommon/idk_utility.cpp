#include "idk_utility.h"

int
idk::roundup( int n, int multipleof )
{
    if (n % multipleof == 0)
        return n;
    return n + (multipleof - (n % multipleof));
};

#define SQ(a) ((a)*(a))
float
idk::distanceSq( glm::vec3 a, glm::vec3 b )
{
    return SQ(a.x-b.x) + SQ(a.y-b.y) + SQ(a.z-b.z);
}
#undef SQ



void
idk::print( std::string sep )
{
    std::cout << "\n";
}
