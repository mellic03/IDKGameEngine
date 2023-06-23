#pragma once

#include "../IDK_engine.h"



class Builtin_Scripting: public idk::Module
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
                Builtin_Scripting( std::string name, int idx ): Module(name, idx)   {  };
    void        init(idk::Engine &)     {  };
    void        stage_A(idk::Engine &)  {  };
    void        stage_B(idk::Engine &)  {  };

};

