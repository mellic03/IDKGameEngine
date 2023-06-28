#pragma once

#include "../IDK_engine.h"


class Lightsource_CS: public idk::ComponentSystem
{
private:
    std::vector<int> _lightsource_ids;  // v[obj_id] = lightsource_id

public:
    void        init( idk::Engine & )       {  };
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );

    void        onAssignment( int obj_id, idk::Engine & );
    void        onGameObjectCreation( int obj_id );
    void        onGameObjectDeletion( int obj_id );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id );

};
