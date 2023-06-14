#pragma once

#include "../IDKGameEngine_common/IDKGameEngine_common.h"





class idk::GameObject
{
protected:
    std::vector<size_t>     _components;

public:
    int         model_id      = -1;
    int         transform_id  = -1;

                GameObject(): _components(0) {  };
                GameObject(int num_components): _components(num_components, 0) {  };

    bool        hasComponent(uint idx);
    void        giveComponent(uint idx);
    void        removeComponent(uint idx);

};
