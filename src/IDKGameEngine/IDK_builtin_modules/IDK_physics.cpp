#include "IDK_physics.h"


idk::builtin_modules::Builtin_Physics::Builtin_Physics()
{
    _predicate = [](idk::GameObject &obj1, idk::GameObject &obj2)
    {
        int x = 5;
    };
}


// void
// idk::builtin_modules::Builtin_Physics::_sphere_sphere_collision(spherecollider &a, spherecollider &b)
// {
    
// }


void
idk::builtin_modules::Builtin_Physics::stage_C(idk::Engine &engine)
{
    // engine.gameObjects().for_each_pair(_predicate);
}

