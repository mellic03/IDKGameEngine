#pragma once

#include "../IDKGameEngine_common/IDKGameEngine_common.h"



class idk::GameObject::Base
{
protected:
    idk::transform          _transform;
    uint                    _id;
    uint                    _transform_id;

public:
                            Base() {  };
    uint                    ID()            { return _id;           };
    uint                    transformID()   { return _transform_id; };
    virtual void            perFrame() = 0;
};


class idk::GameObject::Renderable: public idk::GameObject::Base
{
protected:
    uint                    _model_id;

public:
                            Renderable();
    void                    perFrame() {  };
};


class idk::GameObject::Physical: public idk::GameObject::Base
{
protected:
    uint                    _model_id;

public:
                            Physical() {  };
    void                    perFrame() {  };

};


