#pragma once

#include "IDK_builtin_modules_common.h"


class Grabbable: public idk::Module
{
private:
    std::function<void(int)>    _stage_B_predicate;

public:
                Grabbable( int idx ): Module(idx)  {  };
    void        init( idk::Engine & );
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );

};

