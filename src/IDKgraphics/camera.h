#pragma once

#include "../IDKcore/IDKcore.h"



class idk::Camera
{
private:
    idk::transform _transform;
    idk::mat4 _projection, _view;
    friend class idk::RenderEngine;

public:
    Camera() {  };

    idk::transform &transform() { return _transform; };

};

