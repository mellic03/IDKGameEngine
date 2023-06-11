#pragma once

#include "../IDKGameEngine/IDK_engine.h"


class idk::core_modules::PhysicsModule: public idk::Module
{
private:
    std::function<void(idk::GameObject &, idk::GameObject &)>   _predicate;

                struct spherecollider
                {
                    float radius;
                };

    void        _sphere_sphere_collision(spherecollider &a, spherecollider &b) {  };

public:
                PhysicsModule();
    void        init(idk::Engine &engine)       {  };

    void        stage_A(idk::Engine &engine)    {  };
    void        stage_B(idk::Engine &engine)    {  };
    void        stage_C(idk::Engine &engine);
};

