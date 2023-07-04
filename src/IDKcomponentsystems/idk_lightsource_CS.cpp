#include "idk_lightsource_CS.h"


void
Lightsource_CS::stage_B( idk::Engine &engine )
{
    // idk::RenderEngine &ren = engine.rengine();

    // auto &tCS = engine.getCS<Transform_CS>("transform");
    // auto obj_ids = engine.gameObjects_byComponent(_component_index);
    // for (int obj_id: obj_ids)
    // {
    //     auto &obj_transform = tCS.getTransform(obj_id);
    //     idk::lightsource::Point &plight = ren.pointlights().get(_lightsource_ids[obj_id]);
    //     plight.transform = obj_transform;
    //     plight.transform.scale(glm::vec3(0.2f));
    // }


    // // Draw sphere primitive for every point light
    // ren.pointlights().for_each(
    //     [&ren](idk::lightsource::Point &light)
    //     {
    //         ren.drawModel(ren.solid_shader, ren.SPHERE_PRIMITIVE, light.transform);
    //         ren.setUniform_vec3(ren.solid_shader, "un_color", light.diffuse);
    //     }
    // );

}


void
Lightsource_CS::onGameObjectCreation( int obj_id )
{
    if (obj_id >= _lightsource_ids.size())
        _lightsource_ids.resize(obj_id+1, -1);
    else
        _lightsource_ids[obj_id] = -1;
}


void
Lightsource_CS::onGameObjectDeletion( int obj_id )
{
    _lightsource_ids[obj_id] = -1;
}


void
Lightsource_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id )
{

}


void
Lightsource_CS::onAssignment( int obj_id, idk::Engine &engine )
{
    uint pointlight_id = engine.rengine().createPointLight();
    _lightsource_ids[obj_id] = pointlight_id;
}
