#pragma once

#include "libidk/libidk.h"

namespace idk
{
    struct Pointlight;
    struct Spotlight;
    struct Dirlight;

    enum class lightsource { POINT, SPOT, DIR };

    namespace LightFlag
    {
        enum lightflag_t: uint32_t
        {
            NONE       = 0,
            SHADOWMAP  = 1 << 0,
            VOLUMETRIC = 1 << 1,
        };
    }
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

