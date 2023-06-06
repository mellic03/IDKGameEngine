#pragma once

#include "../../IDKcore/IDKcore.h"


class idk::GameObject::Base
{
protected:
    idk::transform  _transform;
    uint            _id;
    uint            _transform_id;

public:
                    Base() {  };
    uint            ID()            { return _id;           };
    uint            transformID()   { return _transform_id; };
};


class idk::GameObject::Renderable: public idk::GameObject::Base
{
protected:
    uint            _model_id;

public:
                    Renderable() {  };

};


