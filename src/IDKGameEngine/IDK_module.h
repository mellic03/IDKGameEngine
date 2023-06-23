#pragma once

#include "IDKGameEngine_common/IDKGameEngine_common.h"


class idk::Module
{
protected:
    int         _component_index = -1;
    std::string _name;

public:
                    Module(std::string name, int idx)   { _name = name; _component_index = idx; };
    virtual         ~Module() = default;
    std::string     name() const { return _name; };
    virtual void    init( idk::Engine & ) = 0;
    virtual void    stage_A( idk::Engine & ) = 0;
    virtual void    stage_B( idk::Engine & ) = 0;
};


