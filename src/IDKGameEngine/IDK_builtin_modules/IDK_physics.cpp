#include "IDK_physics.h"



void
idk::builtin_modules::Builtin_Physics::init(idk::Engine &engine)
{
    _predicate = [](idk::GameObject &obj1, idk::GameObject &obj2)
    {
        int x = 5;
    };
}


void
idk::builtin_modules::Builtin_Physics::stage_B(idk::Engine &engine)
{
    engine.gameObjects_byComponent(_component_index).for_each(
        [&engine](int obj_id)
        {
            GameObject &obj = engine.getGameObject(obj_id);
            idk::Transform &t = engine.rengine().getTransform(obj.transform_id);
            float dtime = engine.deltaTime();

            if (t.position().y < -5.0f)
                t.velocity().y = 0.0f;
            else
                t.velocity().y -= 0.01f;

            t.position() += dtime * t.velocity();
        }
    );
}


void
idk::builtin_modules::Builtin_Physics::giveSphereCollider( uint obj_id )
{
    _sphere_colliders.add(spherecollider(), obj_id);
}



void
idk::builtin_modules::Builtin_Physics::removeSphereCollider( uint obj_id )
{
    _sphere_colliders.remove(obj_id);
}

