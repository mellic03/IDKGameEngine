#pragma once

#include "../IDK_engine.h"


class Grabbable: public idk::Module
{
private:
    std::function<void(int)>    _stage_B_predicate;

public:
                Grabbable( std::string name, int idx ): Module(name, idx)   {  };
    void        init( idk::Engine & );
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );

};

