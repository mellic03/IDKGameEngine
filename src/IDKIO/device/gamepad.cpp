#include "gamepad.hpp"
#include <libidk/idk_assert.hpp>
#include <libidk/idk_log.hpp>
#include <glm/glm.hpp>

#include <SDL2/SDL.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
using lf = idk::log_flag;


idk::Gamepad::Gamepad()
:   idk::DeviceController(64, 6)
{
    // LOG_INFO("idk::Gamepad::Gamepad");
    LOG_ADV(lf::IO|lf::DETAIL, "");

    lstick   = {0, 0, -1, +1, 0.2, 1.0};
    rstick   = {0, 0, -1, +1, 0.2, 1.0};
    ltrigger = {0, 0,  0, +1, 0.0, 1.0};
    rtrigger = {0, 0,  0, +1, 0.0, 1.0};

    for (int i=0; i<_down.size(); i++)
    {
        _down[i] = false;
        _tapped[i] = false;
        _timers[i] = 0;
    }
}


void
idk::Gamepad::update( uint32_t msElapsed )
{
    bool btn_down = false;

    for (int i=0; i<_down.size(); i++)
    {
        if (_down[i])
        {
            _timers[i] += uint32_t(msElapsed);
        }
    }
}


bool
idk::Gamepad::buttonDown( uint32_t btn )
{
    return _down[btn];
}


bool
idk::Gamepad::buttonTapped( uint32_t btn )
{
    return _tapped[btn];
}





int
idk::Gamepad::_getDataIdx( uint32_t axis )
{
    int idx;

    switch (axis)
    {
        default: idx=-1; break;
        case SDL_CONTROLLER_AXIS_LEFTX:        idx=0;  break;
        case SDL_CONTROLLER_AXIS_LEFTY:        idx=1;  break;
        case SDL_CONTROLLER_AXIS_RIGHTX:       idx=6;  break;
        case SDL_CONTROLLER_AXIS_RIGHTY:       idx=7;  break;
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:  idx=12; break;
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: idx=18; break;
    }
    IDK_ASSERT("Invalid axis", idx != -1);

    return idx;
}


int
idk::Gamepad::_getBoundsIdx( uint32_t axis )
{
    int idx;

    switch (axis)
    {
        default: idx=-1; break;
        case SDL_CONTROLLER_AXIS_LEFTX:        idx=2;  break;
        case SDL_CONTROLLER_AXIS_LEFTY:        idx=2;  break;
        case SDL_CONTROLLER_AXIS_RIGHTX:       idx=8;  break;
        case SDL_CONTROLLER_AXIS_RIGHTY:       idx=8;  break;
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:  idx=14; break;
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: idx=20; break;
    }
    IDK_ASSERT("Invalid axis", idx != -1);

    return idx;
}


int
idk::Gamepad::_getDeadzoneIdx( uint32_t axis )
{
    int idx;

    switch (axis)
    {
        default: idx=-1; break;
        case SDL_CONTROLLER_AXIS_LEFTX:        idx=4;  break;
        case SDL_CONTROLLER_AXIS_LEFTY:        idx=4;  break;
        case SDL_CONTROLLER_AXIS_RIGHTX:       idx=10; break;
        case SDL_CONTROLLER_AXIS_RIGHTY:       idx=10; break;
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:  idx=16; break;
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: idx=22; break;
    }
    IDK_ASSERT("Invalid axis", idx != -1);

    return idx;
}


static uint32_t
SDLAxisToGamepadEvent( uint32_t axis )
{
    using namespace idk;

    switch (axis)
    {
        default: return GamepadEvent::INVALID;
        case SDL_CONTROLLER_AXIS_LEFTX:        return GamepadEvent::LEFT_STICK;
        case SDL_CONTROLLER_AXIS_LEFTY:        return GamepadEvent::LEFT_STICK;
        case SDL_CONTROLLER_AXIS_RIGHTX:       return GamepadEvent::RIGHT_STICK;
        case SDL_CONTROLLER_AXIS_RIGHTY:       return GamepadEvent::RIGHT_STICK;
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:  return GamepadEvent::LEFT_TRIGGER;
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return GamepadEvent::RIGHT_TRIGGER;
    }
}


void
idk::Gamepad::updateAxis( uint8_t axis, int16_t value )
{
    // LOG_ADV(lf::IO|lf::DETAIL, "axis {} {}", int(axis), int(value));

    static constexpr int axis_min = -32768;
    static constexpr int axis_max = +32767;

    int idx;
    float minv, maxv, mindz, innerDZ, outerDZ;

    idx  = _getBoundsIdx(axis);
    minv = data[idx+0];
    maxv = data[idx+1];
    
    idx     = _getDeadzoneIdx(axis);
    innerDZ = data[idx+0];
    outerDZ = data[idx+1];

    float alpha = (float(value)-axis_min) / (axis_max-axis_min);

    if (axis <= SDL_CONTROLLER_AXIS_RIGHTY)
    {
        float distFromCenter = 2.0f * glm::abs(0.5f - alpha);
        float scale = glm::clamp((distFromCenter-innerDZ) / (outerDZ-innerDZ), 0.0f, 1.0f);
    
        alpha = scale * 2.0f * (alpha - 0.5f); // [0, 1] --> [-0.5, +0.5]
        alpha = 0.5f * alpha + 0.5f;           // [-0.5, +0.5] --> [0, 1]
    }

    else
    {
        alpha = float(value) / axis_max;
    }

    data[_getDataIdx(axis)] = glm::mix(minv, maxv, alpha);
    this->emit(SDLAxisToGamepadEvent(axis));
}



void
idk::Gamepad::updateButton( uint32_t btn, bool dwn )
{
    // LOG_ADV(lf::IO|lf::DETAIL, "button {} {}", btn, (dwn ? "down" : "up"));

    uint32_t idx = btn;
    DeviceController::updateButton(idx, dwn);

    uint32_t flags = 0;
    if (dwn) flags |= GamepadEvent::BTN_DWN;
    else     flags |= GamepadEvent::BTN_UP;

    this->emit(flags | btn);
}


void
idk::Gamepad::_clear()
{
    lstick.x = 0.0f;
    lstick.y = 0.0f;
    rstick.x = 0.0f;
    rstick.y = 0.0f;
}
