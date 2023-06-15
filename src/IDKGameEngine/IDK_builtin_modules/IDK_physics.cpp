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
        [&engine](int idx)
        {
            GameObject &obj = engine.getGameObject(idx);
            engine.rengine().getTransform(obj.transform_id).translate(glm::vec3(0.0f, 0.001f, 0.0f));
        }
    );
}

