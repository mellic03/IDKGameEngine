#pragma once

#include "../IDK_builtin_modules_common.h"




class idk::builtin_modules::Builtin_UI: public idk::Module
{
private:
    int         _width;
    int         _height;
    void        _rect(int x, int y, int w, int h);

public:
                Builtin_UI() {  };
    void        init(idk::Engine &engine)       {  };
    void        stage_A(idk::Engine &engine)    {  };
    void        stage_B(idk::Engine &engine)    {  };
    void        stage_C(idk::Engine &engine);
};


