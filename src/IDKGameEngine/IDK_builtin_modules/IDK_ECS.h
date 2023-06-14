#pragma once

#include "IDK_builtin_modules_common.h"


class idk::builtin_modules::Builtin_ECS: public idk::Module
{
private:
    // idk::vector<model>   _model_components;
    // void                 _model_system(idk::Engine &)


public:
                Builtin_ECS();
    void        init(idk::Engine &)       {  };
    void        stage_A(idk::Engine &)    {  };
    void        stage_B(idk::Engine &);

};



