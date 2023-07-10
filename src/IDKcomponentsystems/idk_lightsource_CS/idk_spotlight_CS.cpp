#include "idk_spotlight_CS.h"
#include "../idk_transform_CS.h"



void
SpotLight_CS::init( idk::Engine &engine )
{
    addDependencies("transform");
    _engineptr = &engine;
}


void
SpotLight_CS::stage_B( idk::Engine &engine )
{
    idk::RenderEngine &ren = engine.rengine();

    auto &tCS = engine.getCS<Transform_CS>("transform");
    auto obj_ids = engine.gameObjects_byComponent(_component_index);
    for (int obj_id: obj_ids)
    {
        auto &obj_transform = tCS.getTransform(obj_id);
        idk::lightsource::Spot &spotlight = ren.spotlights().get(_spotlight_ids[obj_id]);
        spotlight.position = glm::vec4(obj_transform.position(), 1.0f);

        if (_draw_spotlight[obj_id])
        {
            ren.drawModel(ren.solid_shader, ren.SPHERE_PRIMITIVE, obj_transform);
        }        
    }

}


void
SpotLight_CS::onGameObjectCreation( int obj_id, idk::Engine &engine )
{
    if (obj_id >= _spotlight_ids.size())
    {
        _draw_spotlight.resize(obj_id+1, false);
        _spotlight_ids.resize(obj_id+1, -1);
    }
    else
    {
        _draw_spotlight[obj_id] = false;
        _spotlight_ids[obj_id] = -1;
    }
}


void
SpotLight_CS::onGameObjectDeletion( int obj_id, idk::Engine &engine )
{
    _spotlight_ids[obj_id] = -1;
}


void
SpotLight_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    uint spotlight_id = engine.rengine().createSpotlight();
    _spotlight_ids[dest_obj_id] = spotlight_id; 
}


void
SpotLight_CS::onAssignment( int obj_id, idk::Engine &engine )
{
    uint spotlight_id = engine.rengine().createSpotlight();
    _spotlight_ids[obj_id] = spotlight_id;
}
