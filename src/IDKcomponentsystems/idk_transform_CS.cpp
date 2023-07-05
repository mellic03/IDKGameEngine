#include "idk_transform_CS.h"


void
Transform_CS::onGameObjectCreation( int obj_id )
{
    if (obj_id >= _transforms.size())
        _transforms.resize(obj_id+1);
    else
        _transforms[obj_id] = idk::Transform();
}


void
Transform_CS::onGameObjectDeletion( int obj_id )
{
    _transforms[obj_id] = idk::Transform();
}


void
Transform_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id )
{
    // _transforms[dest_obj_id] = _transforms[src_obj_id];
}


void 
Transform_CS::translate( int obj_id, glm::vec3 v )
{
    _transforms[obj_id].translate(v);
}

