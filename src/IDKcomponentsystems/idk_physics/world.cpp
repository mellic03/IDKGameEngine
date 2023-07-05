#include "world.h"


void
idk_physics::World::init()
{
    _config         = new btDefaultCollisionConfiguration();
    _dispatcher     = new btCollisionDispatcher(_config);
    _paircache      = new btDbvtBroadphase();
    _solver         = new btSequentialImpulseConstraintSolver;
    _dynamicsWorld  = new btDiscreteDynamicsWorld( _dispatcher,
                                                _paircache,
                                                _solver,
                                                _config ); 

    _dynamicsWorld->setGravity(btVector3(0, -10, 0));
};


void
idk_physics::World::addBoxCollider(idk_physics::BoxCollider box)
{
    _dynamicsWorld->addRigidBody(box.rigidbody);
};


void
idk_physics::World::step(float delta_time)
{
    _dynamicsWorld->stepSimulation(delta_time, 1);
};


std::vector<btCollisionObject *>
idk_physics::World::getObjects()
{
    std::vector<btCollisionObject *> objects;

    for (int j =_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; --j)
    {
        btCollisionObject *obj = _dynamicsWorld->getCollisionObjectArray()[j];
        objects.push_back(obj);
    }

    return objects;
}
