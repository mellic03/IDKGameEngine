#pragma once

#include "idk_components.hpp"

#include "sys-audio.hpp"
#include "sys-camera.hpp"
#include "sys-lightsource.hpp"
#include "sys-model.hpp"
#include "sys-particle.hpp"
#include "sys-physics.hpp"
#include "sys-rendersetting.hpp"
#include "sys-transform.hpp"


namespace idk
{
    void registerComponents();
    void registerSystems();
};
