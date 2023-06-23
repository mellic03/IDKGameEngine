#pragma once

#include "IDKgraphics_common/IDKgraphics_common.h"


class idk::Camera
{
private:
    idk::Transform      _transform;
    glm::mat4           _projection;
    glm::mat4           _view;

    glm::vec3           _right;
    glm::vec3           _front;
    glm::vec3           _up;

    glm::vec3           _default_pos;
    glm::vec4           _default_right;
    glm::vec4           _default_front;
    glm::vec4           _default_up;



    float               _fov;
    float               _near;
    float               _far;

    bool                _ylock  = false;
    bool                _noroll = false;


                        friend class idk::RenderEngine;

public:
                        Camera(float fov, float near, float far);
                        Camera();

    idk::Transform &    transform()     { return _transform;  };
    glm::mat4 &         projection()    { return _projection; };
    glm::mat4           view();

    void                translate(glm::vec3 v);
    void                elevation(float f);

    void                pitch(float f);
    void                roll(float f);
    void                yaw(float f);

    glm::vec3           front() const { return _front; };

    void                aspect(float width, float height);
    void                ylock(bool lock)  { _ylock = lock;  };
    void                noroll(bool lock) { _noroll = lock; };

};

