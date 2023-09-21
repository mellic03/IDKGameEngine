#include "idk_lightsource.h"


idk::Pointlight::Pointlight():
    position    (0.0f, 0.0f, 0.0f, 1.0f),
    ambient     (0.01f),
    diffuse     (1.0f),
    attenuation (0.0f, 1.0f, 0.0f, 0.0f)
{

}


idk::Spotlight::Spotlight():
    position        (0.0f, 0.0f, 0.0f, 1.0f),
    direction       (0.0f),
    ambient         (0.01f),
    diffuse         (5.0f, 5.0f, 6.0f, 0.0f),
    attenuation     (0.0f, 0.0f, 2.0f, 0.0f),
    cutoff          (glm::radians(35.5f), glm::radians(45.5f), 0.0f, 0.0f)
{
    
}


idk::Dirlight::Dirlight():
    direction   (1.0f, -2.0f, 0.0, 0.0f),
    ambient     (0.01f),
    diffuse     (0.01f, 0.01f, 0.012f, 0.0f)
{
    direction = glm::normalize(direction);
}

