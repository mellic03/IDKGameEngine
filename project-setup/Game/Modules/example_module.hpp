#pragma once

#include "IDKengine.h"


class ExampleModule: public idk::Module
{
private:

public:
    void        init    ( idk::Engine & ) {  };
    void        stage_A ( idk::Engine & ) {  };
    void        stage_B ( idk::Engine & ) {  };
    void        stage_C ( idk::Engine & ) {  };

};

