#pragma once

#include "../IDK_engine.h"


class SphereCollider_CS: public idk::ComponentSystem
{
private:

    std::vector<float>  _radii;

public:

    void        init( idk::Engine & )       {  };
    void        stage_A( idk::Engine & );
    void        stage_B( idk::Engine & )    {  };

    void        onGameObjectCreation( int obj_id );
    void        onGameObjectDeletion( int obj_id );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id );

    void        setRadius( uint obj_id, float radius )  { _radii[obj_id] = radius; };
};

