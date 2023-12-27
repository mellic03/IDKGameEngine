#include "idk_OBB.hpp"

#include <iostream>


idk::OBB
idk::OBB_to_NDC( const glm::mat4 &proj_view, const idk::OBB &obb )
{
    idk::OBB out;

    for (int i=0; i<8; i++)
    {
        // std::cout << obb.corners[i].x << "  " << obb.corners[i].y << "  " << obb.corners[i].z << "\n";

        out.corners[i] = proj_view * obb.corners[i];
        out.corners[i] /= out.corners[i].w;
    }

    return out;
}


idk::OBB::OBB( const glm::vec2 &x, const glm::vec2 &y, const glm::vec2 &z )
{
    for (int i=0; i<2; i++)
    {
        for (int j=0; j<2; j++)
        {
            for (int k=0; k<2; k++)
            {
                corners[2*2*i + 2*j + k] = glm::vec4(x[i], y[j], z[k], 1.0f);
            }
        }
    }
}


