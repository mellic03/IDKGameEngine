#pragma once

#include "IDKcommon/IDKcommon.h"

namespace idk::lightsource
{
    struct Point;
    struct Spot;
    struct Dir;
};


struct idk::lightsource::Point
{
    glm::vec4       position;
    glm::vec4       ambient;
    glm::vec4       diffuse;
    glm::vec4       attenuation;
                    Point();
};


struct idk::lightsource::Spot
{
    glm::vec4       position;
    glm::vec4       direction;
    glm::vec4       ambient;
    glm::vec4       diffuse;
    glm::vec4       attenuation;
    float           inner_cutoff;
    float           outer_cutoff;
                    Spot();
};


struct idk::lightsource::Dir
{
    Transform       transform;

    glm::vec3       ambient;
    glm::vec3       diffuse;

                    Dir(): ambient(0.1f), diffuse(1.0f) {  };

};

