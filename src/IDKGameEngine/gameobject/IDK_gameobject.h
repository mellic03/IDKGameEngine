#pragma once

#include "../IDKGameEngine_common/IDKGameEngine_common.h"





class idk::GameObject
{
protected:
    idk::vector<size_t>     _components;
    vector<vector<uint>>    _component_ids;

public:
                enum class Component: uint
                {
                    Rendering       = 1 << 0,
                    Physics         = 1 << 1,
                    NUM_COMPONENTS  = 2
                };

                GameObject(): _components(static_cast<uint>(Component::NUM_COMPONENTS), 0) {  };

    bool        hasComponent(Component cmp);
    void        giveComponent(Component cmp);
    void        removeComponent(Component cmp);

};
