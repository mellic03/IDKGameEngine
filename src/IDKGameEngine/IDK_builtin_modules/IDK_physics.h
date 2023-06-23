#pragma once

#include "../IDK_engine.h"


class Builtin_Physics: public idk::Module
{
private:
    std::function<void(idk::GameObject &, idk::GameObject &)>   _predicate;

    std::vector<float>               _masses;
    std::vector<glm::vec3>           _velocities;

public:
                Builtin_Physics( std::string name, int idx ): Module(name, idx)   {  };
    void        init( idk::Engine & );
    void        stage_A( idk::Engine & )    {  };
    void        stage_B( idk::Engine & );

};
