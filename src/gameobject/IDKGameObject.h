#pragma once

#include "../IDKcore/IDKcore.h"
#include <iostream>

class idk::GameObject::Base
{
protected:
    idk::transform  _transform;
    int             _id;
    int             _transform_id;

public:
                    Base() {  };
    int             ID()            { return _id;           };
    int             transformID()   { return _transform_id; };
};


class idk::GameObject::Renderable: public idk::GameObject::Base
{
protected:
    int             _model_id;

public:
                    Renderable() {  };

};

