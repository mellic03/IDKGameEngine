#include "idk_transform_CS.h"


void
Transform_CS::onAssignment( int obj_id, idk::Engine &engine )
{
    _transform_IDs[obj_id] = _transforms.add();
}


void
Transform_CS::onGameObjectCreation( int obj_id, idk::Engine &engine )
{
    if (obj_id >= _transform_IDs.size())
    {
        _transform_IDs.resize(obj_id+1, -1);
    }

    else
    {
        getTransform(obj_id) = idk::Transform();
        _transform_IDs[obj_id] = -1;
    }
}


void
Transform_CS::onGameObjectDeletion( int obj_id, idk::Engine &engine )
{
    _transforms.remove(_transform_IDs[obj_id]);
    _transform_IDs[obj_id] = -1;
}


void
Transform_CS::onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    _transform_IDs[dest_obj_id] = _transforms.add();
}


void 
Transform_CS::translate( int obj_id, glm::vec3 v )
{
    #ifdef IDK_DEBUG
        if (_transform_IDs[obj_id] == -1)
        {
            std::cout << "_transform_IDs[" << obj_id << "] == -1" << std::endl;
            exit(1);
        }
    #endif

    _transforms.get(_transform_IDs[obj_id]).translate(v);
}

