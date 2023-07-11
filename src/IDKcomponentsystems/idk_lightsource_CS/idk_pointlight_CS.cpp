#include "idk_pointlight_CS.h"
#include "../idk_transform_CS.h"


void
PointLight_CS::init( idk::Engine &engine )
{
    addDependencies("transform");
    _engineptr = &engine;
}


void
PointLight_CS::stage_B( idk::Engine &engine )
{
    idk::RenderEngine &ren = engine.rengine();

    auto &tCS = engine.getCS<Transform_CS>("transform");
    auto obj_ids = engine.gameObjects_byComponent(_component_index);
    for (int obj_id: obj_ids)
    {
        auto &obj_transform = tCS.getTransform(obj_id);
        idk::lightsource::Point &plight = ren.pointlights().get(_pointlight_ids[obj_id]);
        plight.position = glm::vec4(obj_transform.position(), 1.0f);

        ren.drawModel(ren.solid_shader, ren.SPHERE_PRIMITIVE, obj_transform);
    }
}


void
PointLight_CS::onGameObjectCreation( int obj_id, idk::Engine &engine )
{
    if (obj_id >= _pointlight_ids.size())
        _pointlight_ids.resize(obj_id+1, -1);
    else
        _pointlight_ids[obj_id] = -1;
}


void
PointLight_CS::onGameObjectDeletion( int obj_id, idk::Engine &engine )
{
    _pointlight_ids[obj_id] = -1;
}


void
PointLight_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    uint pointlight_id = engine.rengine().createPointlight();
    _pointlight_ids[dest_obj_id] = pointlight_id; 
}


void
PointLight_CS::onAssignment( int obj_id, idk::Engine &engine )
{
    uint pointlight_id = engine.rengine().createPointlight();
    _pointlight_ids[obj_id] = pointlight_id;
}
