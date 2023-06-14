#pragma once

#include "IDK_builtin_modules_common.h"



class idk::builtin_modules::Builtin_Scripting
{
public:
                struct script
                {
                    std::string filepath;
                };

    /*
        Entire lua interface could be defined here
    */

private:


public:
                Builtin_Scripting()     {  };
    void        init(idk::Engine &)     {  };
    void        stage_A(idk::Engine &)  {  };
    void        stage_B(idk::Engine &)  {  };

};

