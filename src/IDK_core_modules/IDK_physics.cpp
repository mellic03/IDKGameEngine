#include "IDK_physics.h"


idk::PhysicsModule::PhysicsModule()
{
    _predicate = [](idk::GameObject &obj1, idk::GameObject &obj2)
    {
        int x = 5;
    };
}




void
idk::PhysicsModule::_sphere_sphere_collision(spherecollider &a, spherecollider &b)
{
    
}



void
idk::PhysicsModule::update(idk::Engine &engine)
{
    engine.gameObjects().for_each_pair(_predicate);
}

