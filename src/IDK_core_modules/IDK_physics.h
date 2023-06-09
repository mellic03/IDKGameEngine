#pragma once

#include "../IDKGameEngine/IDK_engine.h"


class idk::PhysicsModule: public idk::Module
{
private:
    std::function<void(idk::GameObject &, idk::GameObject &)>   _predicate;


                struct spherecollider
                {
                    float radius;
                };

    void        _sphere_sphere_collision(spherecollider &a, spherecollider &b);

public:
                PhysicsModule();
    void        update(idk::Engine &engine);
};

