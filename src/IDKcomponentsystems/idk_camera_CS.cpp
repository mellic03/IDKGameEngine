#include "idk_camera_CS.h"
#include "idk_transform_CS.h"


void
Camera_CS::init( idk::Engine &engine )
{
    addDependencies("transform, physics");
}


void
Camera_CS::stage_A( idk::Engine &engine )
{
    Transform_CS &tCS = engine.getCS<Transform_CS>("transform");

    if (_current_cammed_object == -1)
        return;

    idk::Camera &camera = engine.rengine().getCamera();
    camera.transform() = tCS.getTransform(_current_cammed_object);
}


void
Camera_CS::onAssignment( int obj_id, idk::Engine &engine )
{
    _current_cammed_object = obj_id;
}


void
Camera_CS::onGameObjectCreation( int obj_id, idk::Engine &engine )
{

}


void
Camera_CS::onGameObjectDeletion( int obj_id, idk::Engine &engine )
{
    if (obj_id == _current_cammed_object)
        _current_cammed_object = -1;
}


void
Camera_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{

}

