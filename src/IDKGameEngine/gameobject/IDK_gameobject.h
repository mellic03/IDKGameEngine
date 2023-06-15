#pragma once

#include "../IDKGameEngine_common/IDKGameEngine_common.h"





class idk::GameObject
{
protected:

public:
    std::vector<int> components;

    int model_id      = -1;
    int transform_id  = -1;

    GameObject(): components(0) {  };
    GameObject(int num_components): components(num_components, 0) {  };
};
