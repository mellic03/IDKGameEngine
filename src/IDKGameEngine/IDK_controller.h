#pragma once

#include "IDKGameEngine_common/IDKGameEngine_common.h"


class idk::Controller
{
private:
    idk::transform *    _transform;

public:
                Controller();
    void        control(idk::GameObject &object);
    void        control(idk::transform &t);

};

