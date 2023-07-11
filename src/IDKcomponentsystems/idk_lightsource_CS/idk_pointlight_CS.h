#pragma once

#include "../IDK_builtin_CS_common.h"


class PointLight_CS: public idk::ComponentSystem
{
private:
    std::vector<int> _pointlight_ids;  // v[obj_id] = lightsource_id
    idk::Engine *_engineptr;

public:
    void        init( idk::Engine & );
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );

    void        onAssignment( int obj_id, idk::Engine & );
    void        onGameObjectCreation( int obj_id, idk::Engine & );
    void        onGameObjectDeletion( int obj_id, idk::Engine & );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );
    
    
    idk::lightsource::Point &
    getPointlight( int obj_id )
    {
        return _engineptr->rengine().pointlights().get(_pointlight_ids[obj_id]);
    };

};
