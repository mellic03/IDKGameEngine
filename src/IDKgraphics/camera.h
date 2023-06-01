#pragma once

#include "../IDKcore/IDKcore.h"



class IDK::Camera
{
private:
    IDK::transform _transform;
    IDK::mat4 _projection, _view;
    friend class IDK::RenderEngine;

public:
    Camera() {  };

    IDK::transform &transform() { return _transform; };

};

