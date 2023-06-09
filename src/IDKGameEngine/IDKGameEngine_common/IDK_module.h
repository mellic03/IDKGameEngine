#pragma once

#include "../../IDKcore/IDKcore.h"


class idk::Module
{
protected:

public:
    virtual void update(idk::Engine &engine) = 0;
};


