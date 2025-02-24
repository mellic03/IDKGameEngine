#pragma once
#include "gamepad.hpp"


namespace idk
{
    class DS4;
}


class idk::DS4: public idk::Gamepad
{
private:

public:
    DS4();

    virtual bool canBind( idk::Device* ) final;

    enum class TriggerEffect: uint8_t
    {
        RESISTANCE_START,          // resistance start point
        EFFECT_FORCE,              // 
        RANGE_FORCE,               // effects[2]
        NEAR_RELEASE_STRENGTH,     // effects[3]
        NEAR_MIDDLE_STRENGTH,      // effects[4]
        PRESSED_STRENGTH,          // effects[5]
        ACTUATION_FREQUENCY        // effects[6]
    };

    enum class TriggerMode: uint8_t
    {
        OFF      = 0x0,
        RIGID    = 0x1,
        PULSE    = 0x2,
        RIGID_A  = 0x1 | 0x20,
        RIGID_B  = 0x1 | 0x04,
        RIGID_AB = 0x1 | 0x20 | 0x04,
        PULSE_A  = 0x2 | 0x20,
        PULSE_B  = 0x2 | 0x04,
        PULSE_AB = 0x2 | 0x20 | 0x04
    };

};
