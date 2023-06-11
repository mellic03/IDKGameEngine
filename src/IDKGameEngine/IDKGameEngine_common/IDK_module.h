#pragma once

#include "../../IDKcore/IDKcore.h"


class idk::Module
{
protected:

public:
    virtual ~Module() = default;
    virtual void init(idk::Engine &engine) = 0;
    virtual void stage_A(idk::Engine &engine) = 0;
    virtual void stage_B(idk::Engine &engine) = 0;
    virtual void stage_C(idk::Engine &engine) = 0;
};


