#pragma once

#include "idk_components.hpp"
#include "idk_systems.hpp"
#include "sys-transform.hpp"

#include "sys-physics.hpp"
// #include "sys-physx.hpp"

#include "sys-audio.hpp"
#include "sys-planet.hpp"
#include "sys-gamestate.hpp"
#include "sys-player.hpp"
#include "sys-skeleton.hpp"


namespace idk
{
    void registerComponents( idk::ecs::ECS &ecs );
    void registerSystems( idk::ecs::ECS &ecs );
};
