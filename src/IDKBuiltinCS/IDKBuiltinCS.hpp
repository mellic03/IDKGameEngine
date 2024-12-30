#pragma once

#include "idk_components.hpp"

#include "sys-audio.hpp"
#include "sys-camera.hpp"
#include "sys-lightsource.hpp"
#include "sys-model.hpp"
#include "sys-particle.hpp"
#include "sys-physics.hpp"
#include "sys-rendersetting.hpp"
#include "sys-script.hpp"
#include "sys-transform.hpp"
#include "sys-trigger.hpp"


namespace idk
{
    class ECS;

    void registerComponents( idk::ECS& );
    void registerSystems( idk::ECS& );
};
