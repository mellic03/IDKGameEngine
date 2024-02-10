#pragma once

#include "idk_components.hpp"
#include "idk_systems.hpp"
#include "sys-physx.hpp"
#include "sys-audio.hpp"


namespace idk
{
    void registerComponents( idecs::ECS &ecs );
    void registerSystems( idecs::ECS &ecs );
};
