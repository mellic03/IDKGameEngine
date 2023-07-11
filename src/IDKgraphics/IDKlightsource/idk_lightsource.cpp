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
    diffuse                 = glm::vec4(5.0f, 5.0f, 6.0f, 0.0f);
    attenuation             = glm::vec4(0.0f, 0.0f, 2.0f, 0.0f);
    cutoff                  = glm::vec4(glm::radians(35.5f), glm::radians(45.5f), 0.0f, 0.0f);
}

idk::lightsource::Dir::Dir()
{
    direction               = glm::normalize(glm::vec4(-1.0f, -1.0f, 0.0, 0.0f));
    ambient                 = glm::vec4(0.01f);
    diffuse                 = glm::vec4(1.2f, 1.0f, 1.0f, 0.0f);
}

