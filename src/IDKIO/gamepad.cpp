#include "gamepad.hpp"
#include <libidk/idk_assert.hpp>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <string>



idk::Gamepad::Gamepad()
{
    lstick   = {0, 0, -1, +1, 0.2, 1.0};
    rstick   = {0, 0, -1, +1, 0.2, 1.0};
    ltrigger = {0, 0,  0, +1, 0.0, 1.0};
    rtrigger = {0, 0,  0, +1, 0.0, 1.0};

    for (int i=0; i<down.size(); i++)
    {
        down[i] = false;
        tapped[i] = false;
        timers[i] = 0;
    }
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


void
idk::Gamepad::setAxis( uint32_t axis, int16_t value )
{
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
}


void
idk::Gamepad::setBtn( uint32_t btn, bool is_down )
{
    bool was_down = this->down[btn];
    bool is_up    = !is_down;

    if (was_down && is_up)
    {
        if (this->timers[btn] < 250)
        {
            this->tapped[btn] = true;
        }
    }

    if (is_up)
    {
        this->down[btn] = false;
        this->timers[btn] = 0;
    }

    else if (is_down)
    {
        this->down[btn] = true;
        this->tapped[btn] = false;
    }
}


void
idk::Gamepad::_clear()
{
    lstick.x = 0.0f;
    lstick.y = 0.0f;
    rstick.x = 0.0f;
    rstick.y = 0.0f;
}


bool
idk::Gamepad::isOpen()
{
    return ctl != nullptr;
}


void
idk::Gamepad::open( int index )
{
    ctl = SDL_GameControllerOpen(index);
    id  = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(ctl));
    _clear();
}

void
idk::Gamepad::close()
{
    SDL_GameControllerClose(ctl);
    ctl = nullptr;
}


