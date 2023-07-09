#pragma once


#include "IDK_builtin_CS_common.h"


class Model_CS: public idk::ComponentSystem
{
private:
    GLuint              _default_shader;
    std::vector<int>    _model_ids;
    std::vector<GLuint> _shader_ids;

public:
    void        init(idk::Engine &);
    void        stage_A(idk::Engine &);
    void        stage_B(idk::Engine &)  {  };

    void        onAssignment( int obj_id, idk::Engine & ) {  };
    void        onGameObjectCreation( int obj_id, idk::Engine & );
    void        onGameObjectDeletion( int obj_id, idk::Engine & );
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & );

    void        useModel( int obj_id, int model_id, GLuint shader_id )
    {
        _model_ids[obj_id] = model_id;
        _shader_ids[obj_id] = shader_id;
    };

};

