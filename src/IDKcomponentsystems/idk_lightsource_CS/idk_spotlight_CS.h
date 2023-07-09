#pragma once

#include "../IDK_builtin_CS_common.h"


class SpotLight_CS: public idk::ComponentSystem
{
private:
    std::vector<bool> _draw_spotlight;
    std::vector<int> _spotlight_ids;  // v[obj_id] = lightsource_id

    idk::Engine *   _engineptr;

public:
    void        init( idk::Engine & );
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );

    void        onAssignment( int obj_id, idk::Engine & );
    void        onGameObjectCreation( int obj_id, idk::Engine & );
    void        onGameObjectDeletion( int obj_id, idk::Engine & );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    void        visualize( int obj_id, bool draw ) { _draw_spotlight[obj_id] = draw; };

    idk::lightsource::Spot &getSpotlight( int obj_id ) { return _engineptr->rengine().spotlights().get(_spotlight_ids[obj_id]); };
};
