#include "IDK_physics.h"



void
Builtin_Physics::init(idk::Engine &engine)
{
    _predicate = [](idk::GameObject &obj1, idk::GameObject &obj2)
    {
        int x = 5;
    };
}


void
Builtin_Physics::stage_B(idk::Engine &engine)
{
    std::vector<glm::vec3> &velocities = _velocities;
    std::vector<int> obj_ids = engine.gameObjects_byComponent(_component_index);

    for (int obj_id: obj_ids)
    {
        if (obj_id >= velocities.size())
            velocities.resize(obj_id+1);

        idk::GameObject &obj = engine.getGameObject(obj_id);
        idk::Transform &t = engine.rengine().getTransform(obj.transform_id);
        float dtime = engine.deltaTime();

        velocities[obj_id].y -= 0.01f;

        t.position() += dtime * velocities[obj_id];
    }
}

