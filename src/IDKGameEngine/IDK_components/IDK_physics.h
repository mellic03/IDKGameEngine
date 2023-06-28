#pragma once

#include "../IDK_engine.h"


class Physics_CS: public idk::ComponentSystem
{
private:
    std::vector<float>               _masses;
    std::vector<glm::vec3>           _velocities;

public:
    void        init( idk::Engine & )       {  };
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );


    void        onGameObjectCreation( int obj_id );
    void        onGameObjectDeletion( int obj_id );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id );


    glm::vec3 & getVelocity( int obj_id );

};
