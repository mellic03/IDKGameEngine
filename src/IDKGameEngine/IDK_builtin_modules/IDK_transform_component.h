#pragma once

#include "IDK_builtin_modules_common.h"


class transform_CS: public idk::Module
{
private:
    std::vector<idk::Transform>     _transforms;

public:
                transform_CS( int idx ): Module(idx) {  };

    void        init( idk::Engine & ) {  };
    void        stage_A( idk::Engine & ) {  };
    void        stage_B( idk::Engine & ) {  };

};

