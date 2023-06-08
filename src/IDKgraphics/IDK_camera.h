#pragma once

#include "IDKgraphics_common/IDKgraphics_common.h"


class idk::Camera
{
private:
    idk::transform      _transform;
    glm::mat4           _projection;
    glm::mat4           _view;

    glm::vec3           _front;
    glm::vec3           _right;

    float               _fov;
    float               _near;
    float               _far;
    float               _pitch = 0.0f;
    glm::quat           _dpitch, _dyaw;
    float               _rot_speed = 0.0000001f;

                        friend class idk::RenderEngine;

public:
                        Camera(float fov, float near, float far);
                        Camera();

    idk::transform &    transform()     { return _transform;  };
    glm::mat4 &         projection()    { return _projection; };
    glm::mat4           view();

    void                pitch(float f);
    void                yaw(float f);

};

