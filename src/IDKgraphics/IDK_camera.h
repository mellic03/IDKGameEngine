#pragma once

#include "IDKgraphics_common/IDKgraphics_common.h"


class idk::Camera
{
private:
    idk::transform      _transform;
    glm::mat4           _projection;
    glm::mat4           _view;

                        friend class idk::RenderEngine;

public:
                        Camera();

    idk::transform &    transform() { return _transform; };

};

