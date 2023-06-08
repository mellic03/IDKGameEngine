#pragma once

#include "../IDKGameEngine_common/IDKGameEngine_common.h"





class idk::GameObject
{
protected:
    idk::vector<size_t> _components;

public:

                enum class Component: uint
                {
                    Rendering       = 1 << 0,
                    Physics         = 1 << 1,
                    NUM_COMPONENTS  = 2
                };

                GameObject(): _components(static_cast<uint>(Component::NUM_COMPONENTS), 0) {  };

    bool        hasComponent(Component cmp)     { return _components[static_cast<uint>(cmp)] > 0; };
    void        giveComponent(Component cmp)    { _components[static_cast<uint>(cmp)] += 1;       };
    void        removeComponent(Component cmp)  { _components[static_cast<uint>(cmp)] -= 1;       };

};
