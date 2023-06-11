#pragma once

#include "../../IDKGameEngine/IDK_engine.h"




class idk::core_modules::UIModule: public idk::Module
{
private:
    int         _width;
    int         _height;
    void        _rect(int x, int y, int w, int h);

public:
                UIModule() {  };
    void        init(idk::Engine &engine)       {  };
    void        stage_A(idk::Engine &engine)    {  };
    void        stage_B(idk::Engine &engine)    {  };
    void        stage_C(idk::Engine &engine);
};


