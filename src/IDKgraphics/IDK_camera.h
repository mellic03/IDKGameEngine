#pragma once

#include "IDKgraphics_common/IDKgraphics_common.h"


class idk::Camera
{
private:
    idk::transform      _transform;
    glm::mat4           _projection;
    glm::mat4           _view;

    float               _fov;
    float               _near;
    float               _far;

                        friend class idk::RenderEngine;

public:
                        Camera(float fov, float near, float far);
                        Camera();

    idk::transform &    transform()     { return _transform;  };
    glm::mat4 &         projection()    { return _projection; };
    glm::mat4 &         view()          { return _view;       };

};

