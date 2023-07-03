#pragma once


#include "IDK_builtin_CS_common.h"


class Model_CS: public idk::ComponentSystem
{
private:
    GLuint              _default_shader;
    std::vector<int>    _model_ids;

public:
    void        init(idk::Engine &);
    void        stage_A(idk::Engine &);
    void        stage_B(idk::Engine &)  {  };

    void        onGameObjectCreation( int obj_id );
    void        onGameObjectDeletion( int obj_id );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id );

    void        useModel( int obj_id, int model_id ) { _model_ids[obj_id] = model_id; };

};

