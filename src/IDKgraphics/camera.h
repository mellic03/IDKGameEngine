#pragma once

#include "../IDKcore/IDKcore.h"



class IDK::Camera
{
private:
    IDK::transform _transform;
    IDK::mat4 _projection, _view;
    friend class IDK::Renderer;

public:
    Camera() {  };

    IDK::transform &transform() { return _transform; };

};

