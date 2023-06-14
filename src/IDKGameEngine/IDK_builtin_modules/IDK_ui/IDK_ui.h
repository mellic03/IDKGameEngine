#pragma once

#include "../IDK_builtin_modules_common.h"




class idk::builtin_modules::Builtin_UI: public idk::Module
{
private:
    int         _width;
    int         _height;
    glm::ivec3  _fill_color;
    void        _rect(int x, int y, int w, int h);
    void        _button(int x, int y, int r, int g, int b);
    void        _fill(int r, int g, int b);

public:
                Builtin_UI() {  };
    void        init(idk::Engine &)       {  };
    void        stage_A(idk::Engine &)    {  };
    void        stage_B(idk::Engine &);
};


