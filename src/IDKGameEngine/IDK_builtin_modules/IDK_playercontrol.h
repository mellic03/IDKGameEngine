#pragma once

#include "IDK_builtin_modules_common.h"


class idk::builtin_modules::Builtin_PlayerControl: public idk::Module
{
private:

public:
                Builtin_PlayerControl()           {  };
    void        init(idk::Engine &)       {  };
    void        stage_A(idk::Engine &)    {  };
    void        stage_B(idk::Engine &);
};

