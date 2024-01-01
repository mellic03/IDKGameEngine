#pragma once

#include <libidk/idk_glm.hpp>
#include <libidk/idk_export.hpp>


namespace idk
{
    struct IDK_VISIBLE Pointlight;
    struct IDK_VISIBLE Spotlight;
    struct IDK_VISIBLE Dirlight;

    enum class IDK_VISIBLE lightsource { POINT, SPOT, DIR };

    namespace IDK_VISIBLE LightFlag
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

