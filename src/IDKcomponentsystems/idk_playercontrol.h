#pragma once

#include "IDK_builtin_CS_common.h"


class PlayerControl_CS: public idk::ComponentSystem
{
private:
    int         _current_player_obj = -1;

public:
    void        init(idk::Engine &);
    void        stage_B(idk::Engine &)    {  };
    void        stage_A(idk::Engine &);


    void        onAssignment( int obj_id, idk::Engine & );
    void        onGameObjectCreation( int obj_id, idk::Engine & );
    void        onGameObjectDeletion( int obj_id, idk::Engine & );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

};

