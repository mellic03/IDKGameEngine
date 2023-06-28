#include "IDK_physics.h"
#include "IDK_transform_CS.h"



void
Physics_CS::stage_B(idk::Engine &engine)
{
    Transform_CS *tcomp = engine.getCS<Transform_CS>("transform");

    std::vector<glm::vec3> &velocities = _velocities;
    std::vector<int> obj_ids = engine.gameObjects_byComponent(_component_index);

    for (int obj_id: obj_ids)
    {
        idk::Transform &t = tcomp->getTransform(obj_id);
        t.position() += engine.deltaTime() * velocities[obj_id];

        // glm::vec3 center(0.0f, 10.0f, 0.0f);
        // glm::vec3 dir = 10.0f * glm::normalize(center - t.position());
        // _velocities[obj_id] += engine.deltaTime() * dir;
    }
}


void
Physics_CS::onGameObjectCreation( int obj_id )
{
    if (obj_id >= _velocities.size())
        _velocities.resize(obj_id+1, glm::vec3(0.0f));
    else
        _velocities[obj_id] = glm::vec3(0.0f);
}


void
Physics_CS::onGameObjectDeletion( int obj_id )
{

}


void
Physics_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id )
{

}


glm::vec3 &
Physics_CS::getVelocity( int obj_id )
{
    return _velocities[obj_id];
}
