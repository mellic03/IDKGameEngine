#pragma once

#include "IDK_builtin_CS_common.h"


class PlayerControl_CS: public idk::ComponentSystem
{
private:

public:
    void        init(idk::Engine &)       {  };
    void        stage_B(idk::Engine &)    {  };
    void        stage_A(idk::Engine &);

};
