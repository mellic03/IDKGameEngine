#pragma once

#include "IDKGameEngine_common/IDKGameEngine_common.h"


class idk::Module
{
protected:
    int         _component_index = -1;

public:
                    Module(int idx)         { _component_index = idx; };
    virtual         ~Module() = default;
    virtual void    init( idk::Engine & ) = 0;
    virtual void    stage_A( idk::Engine & ) = 0;
    virtual void    stage_B( idk::Engine & ) = 0;
};


