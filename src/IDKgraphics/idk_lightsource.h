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
    Transform       transform;

    glm::vec3       ambient;
    glm::vec3       diffuse;

    float           radius;
    float           attentuation_constant   = 0.5f;
    float           attentuation_linear     = 0.05f;
    float           attentuation_quadratic  = 0.0f;

                    Point(): ambient(0.1f), diffuse(1.0f) {  };
};


struct idk::lightsource::Spot
{
    Transform       transform;
    glm::vec3       direction;

    glm::vec3       ambient;
    glm::vec3       diffuse;
    float           attentuation_constant   = 0.5f;
    float           attentuation_linear     = 0.25f;
    float           attentuation_quadratic  = 0.0f;

    float           inner_cutoff            = 2.5f;
    float           outer_cutoff            = 45.5f;

                    Spot(): ambient(0.01f), diffuse(1.0f) {  };
};


struct idk::lightsource::Dir
{
    Transform       transform;

    glm::vec3       ambient;
    glm::vec3       diffuse;

                    Dir(): ambient(0.1f), diffuse(1.0f) {  };

};

