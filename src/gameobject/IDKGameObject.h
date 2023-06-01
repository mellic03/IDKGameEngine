#pragma once

#include "../IDKcore/IDKcore.h"
#include <iostream>

class IDK::GameObject::Base
{
protected:
    IDK::transform _transform;
    int _id;

public:
    Base() {  };
    IDK::transform &transform() { return _transform; };

};

