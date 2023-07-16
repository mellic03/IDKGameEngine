#pragma once

#include "IDKcommon/IDKcommon.h"

namespace idk
{
    struct Pointlight;
    struct Spotlight;
    struct Dirlight;
};


struct idk::Pointlight
{
    glm::vec4       position;
    glm::vec4       ambient;
    glm::vec4       diffuse;
    glm::vec4       attenuation;

                    Pointlight();
};


struct idk::Spotlight
{
    glm::vec4       position;
    glm::vec4       direction;
    glm::vec4       ambient;
    glm::vec4       diffuse;
    glm::vec4       attenuation;
    glm::vec4       cutoff;

                    Spotlight();
};


struct idk::Dirlight
{
    glm::vec4       direction;
    glm::vec4       ambient;
    glm::vec4       diffuse;

                    Dirlight();
};

