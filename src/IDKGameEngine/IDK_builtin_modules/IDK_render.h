#pragma once


#include "IDK_builtin_modules.h"


class idk::builtin_modules::Builtin_Render: public idk::Module
{
private:

public:
                Builtin_Render()            {  };
    void        init(idk::Engine &)     {  };
    void        stage_A(idk::Engine &)  {  };
    void        stage_B(idk::Engine &);

};

