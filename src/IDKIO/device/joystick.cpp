#include "joystick.hpp"

#include <SDL2/SDL.h>
#include <libidk/idk_log.hpp>
#include <glm/glm.hpp>

using lf = idk::log_flag;


idk::Joystick::Joystick()
:   DeviceController(32, 4)
{
    LOG_ADV(lf::IO|lf::DETAIL, "");

    bounds.min = {-1, -1, -1,  0};
    bounds.max = {+1, +1, +1, +1};
}


void
idk::Joystick::updateAxis( uint8_t axis, int16_t value )
{
    // LOG_ADV(lf::IO|lf::DETAIL, "Axis: {}", axis);
    this->changed = true;

    static constexpr int16_t axis_min = SDL_JOYSTICK_AXIS_MIN;
    static constexpr int16_t axis_max = SDL_JOYSTICK_AXIS_MAX;

    float alpha = float(value-axis_min) / float(axis_max-axis_min);
    float minv  = bounds.min.data[axis];
    float maxv  = bounds.max.data[axis];

    stick.data[axis] = glm::mix(minv, maxv, alpha);

    switch (axis)
    {
        using JE = JoystickEvent;

        default: return;
        case 0: this->emit(JE::ROLL);     return;
        case 1: this->emit(JE::PITCH);    return;
        case 2: this->emit(JE::YAW);      return;
        case 3: this->emit(JE::THROTTLE); return;
    }
}


void
idk::Joystick::updateHat( uint8_t idx, uint8_t value )
{
    changed = true;
    hat[idx] = value;
}


bool
idk::Joystick::canListen( idk::Device *device )
{
    // if (SDL_IsGameController(prt->which) == SDL_TRUE)
    // {
    //     LOG_ADV(lf::IO|lf::DETAIL, "Cannot bind to Device {}", prt->which);
    //     return false;
    // }

    if (SDL_GameControllerGetType(device->impl) != SDL_CONTROLLER_TYPE_PS4)
    {
        LOG_ADV(lf::IO|lf::DETAIL, "Cannot bind to DS4 on Device {}", device->which);
        return false;
    }

    if (SDL_GameControllerGetType(device->impl) != SDL_CONTROLLER_TYPE_PS5)
    {
        LOG_ADV(lf::IO|lf::DETAIL, "Cannot bind to DS5 on Device {}", device->which);
        return false;
    }

    LOG_ADV(lf::IO|lf::INFO, "Bound to Device {}", device->which);

    return DeviceController::canListen(device);
}
