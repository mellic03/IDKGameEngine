#include "IDK_physics.h"


idk::core_modules::PhysicsModule::PhysicsModule()
{
    _predicate = [](idk::GameObject &obj1, idk::GameObject &obj2)
    {
        int x = 5;
    };
}


// void
// idk::core_modules::PhysicsModule::_sphere_sphere_collision(spherecollider &a, spherecollider &b)
// {
    
// }


void
idk::core_modules::PhysicsModule::stage_C(idk::Engine &engine)
{
    // engine.gameObjects().for_each_pair(_predicate);
}

