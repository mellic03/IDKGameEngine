#pragma once

#include "IDK_builtin_modules_common.h"


class idk::builtin_modules::Builtin_Physics: public idk::Module
{
private:
    std::function<void(idk::GameObject &, idk::GameObject &)>   _predicate;

                struct spherecollider
                {
                    float radius;
                };

    void        _sphere_sphere_collision(spherecollider &a, spherecollider &b) {  };

public:
                Builtin_Physics();
    void        init(idk::Engine &)       {  };
    void        stage_A(idk::Engine &)    {  };
    void        stage_B(idk::Engine &);
};

