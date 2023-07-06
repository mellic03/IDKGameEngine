#pragma once

#include "IDK_builtin_CS_common.h"


class Builtin_UI: public idk::ComponentSystem
{
private:
    int         _width;
    int         _height;
    glm::ivec3  _fill_color;
    void        _rect(int x, int y, int w, int h);
    void        _button(int x, int y, int r, int g, int b);
    void        _fill(int r, int g, int b);

public:
    void        init(idk::Engine &)       {  };
    void        stage_A(idk::Engine &)    {  };
    void        stage_B(idk::Engine &);

    void        onAssignment( int obj_id, idk::Engine & ) {  };
    void        onGameObjectCreation( int obj_id, idk::Engine & ) {  };
    void        onGameObjectDeletion( int obj_id, idk::Engine & ) {  };
    void        onGameObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine & ) {  };

};


