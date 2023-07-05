#include "IDKgraphics/IDKgraphics.h"

#define ENABLE_VHACD_IMPLEMENTATION
#include "VHACD.h"



// void
// Physics_CS::giveMeshCollider( int obj_id, std::vector<idk::Vertex> &vertices )
// {
//     auto &tCS = _engineptr->getCS<Transform_CS>("transform");

//     VHACD::IVHACD *vhacd = VHACD::CreateVHACD();
//     VHACD::IVHACD::Parameters params;
//     params.m_maxConvexHulls = 16;

//     VHACD::SimpleMesh mesh;
// }


int main(int argc, char **argv)
{
    idk::ModelManager manager;

    std::vector<idk::Vertex> vertices;
    manager.loadVertices("assets/models/worm.obj", vertices);

    VHACD::IVHACD *vvv = VHACD::CreateVHACD();

    // VHACD::


    return 0;
}

