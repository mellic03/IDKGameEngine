#pragma once

#include "../IDKGameEngine/IDK_engine.h"


class idk::core_modules::PlayerControlModule: public idk::Module
{
private:

public:
                PlayerControlModule()           {  };
    void        init(idk::Engine &engine)       {  };
    void        stage_A(idk::Engine &engine)    {  };
    void        stage_B(idk::Engine &engine)    {  };
    void        stage_C(idk::Engine &engine);
};

