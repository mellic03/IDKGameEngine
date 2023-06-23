#pragma once

#include "../IDK_engine.h"


class Builtin_UI: public idk::Module
{
private:
    int         _width;
    int         _height;
    glm::ivec3  _fill_color;
    void        _rect(int x, int y, int w, int h);
    void        _button(int x, int y, int r, int g, int b);
    void        _fill(int r, int g, int b);

public:
                Builtin_UI(std::string name, int idx): Module(name, idx)   {  };
    void        init(idk::Engine &)       {  };
    void        stage_A(idk::Engine &)    {  };
    void        stage_B(idk::Engine &);
};


