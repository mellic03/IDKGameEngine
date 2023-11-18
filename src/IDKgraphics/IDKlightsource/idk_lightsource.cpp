#include "idk_lightsource.hpp"


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
    direction   ( 0.63f, -1.0f, -1.0f, 0.0f ),
    // ambient     ( 25.0f / 255.0f,  25.0f  / 255.0f,   12.0f / 255.0f,  0.0f ),
    ambient     ( 0.0f ),
    diffuse     ( 255.0f / 255.0f,  225.0f / 255.0f,  125.0f / 255.0f,  0.0f )
{
    direction = glm::normalize(direction);
}

