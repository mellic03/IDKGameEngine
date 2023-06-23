#pragma once

#include "../IDK_engine.h"


class Builtin_PlayerControl: public idk::Module
{
private:

public:
                Builtin_PlayerControl( std::string name, int idx ): Module(name, idx)   {  };
    void        init(idk::Engine &)       {  };
    void        stage_B(idk::Engine &)    {  };
    void        stage_A(idk::Engine &);

};

