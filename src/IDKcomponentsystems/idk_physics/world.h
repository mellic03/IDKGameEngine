#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include <vector>

#include "rigidbody.h"


namespace idk_physics
{

    class World;

};



class idk_physics::World
{
private:
    btDefaultCollisionConfiguration *       _config;
    btCollisionDispatcher *                 _dispatcher;
    btDbvtBroadphase *                      _paircache;
    btSequentialImpulseConstraintSolver *   _solver;

public:
    btDiscreteDynamicsWorld *               _dynamicsWorld;

    void init();
    void addBoxCollider(idk_physics::BoxCollider box);
    void step(float delta_time);
    std::vector<btCollisionObject *> getObjects();


};

