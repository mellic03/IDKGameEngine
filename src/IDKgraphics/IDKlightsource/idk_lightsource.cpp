#include "idk_lightsource.h"


idk::lightsource::Point::Point()
{
    position                = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    ambient                 = glm::vec4(0.01f);
    diffuse                 = glm::vec4(1.0f);
    attenuation             = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

idk::lightsource::Spot::Spot()
{
    position                = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    direction               = glm::vec4(0.0f);
    ambient                 = glm::vec4(0.01f);
    diffuse                 = glm::vec4(1.0f);
    attenuation             = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    inner_cutoff            = 35.5f;
    outer_cutoff            = 45.5f;
}