#pragma once

#include "../IDK_engine.h"


class Transform_CS: public idk::ComponentSystem
{
private:
    std::vector<idk::Transform>  _transforms;

public:
    void        init( idk::Engine & )       {  };
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & )    {  };

    void        onGameObjectCreation( int obj_id );
    void        onGameObjectDeletion( int obj_id );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id );

    idk::Transform &  getTransform( int obj_id ) { return _transforms[obj_id]; };
    void        translate( int obj_id, glm::vec3 v );
};

