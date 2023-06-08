#pragma once

#include "IDKgraphics_common/IDKgraphics_common.h"



struct idk::lightsource::Point
{
    uint            transform_id;

    glm::vec3       ambient;
    glm::vec3       diffuse;

    float           radius;
    float           attentuation_constant   = 1.0f;
    float           attentuation_linear     = 0.01f;
    float           attentuation_quadratic  = 0.01f;

                    Point() {  };
                    Point(uint tID): ambient(0.05f), diffuse(1.0f), transform_id(tID) {  };
};


struct idk::lightsource::Spot
{
    uint            transform_id;

    glm::vec3       ambient;
    glm::vec3       diffuse;

                    Spot() {  };
                    Spot(uint tID): ambient(0.0f), diffuse(1.0f), transform_id(tID) {  };
};


struct idk::lightsource::Dir
{
    uint            transform_id;

    glm::vec3       ambient;
    glm::vec3       diffuse;

                    Dir() {  };
                    Dir(uint tID): ambient(0.0f), diffuse(1.0f), transform_id(tID) {  };

};

