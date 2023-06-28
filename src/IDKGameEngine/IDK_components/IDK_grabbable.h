#pragma once

#include "../IDK_engine.h"


class Grabbable_CS: public idk::ComponentSystem
{
private:


public:

    void        init( idk::Engine & )       {  };
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );

};

