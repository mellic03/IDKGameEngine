#pragma once
#include "controller.hpp"
#include <libidk/idk_eventemitter.hpp>

namespace idk
{
    class Joystick;

    struct JoystickEvent {
        enum internal_enum: uint32_t {
            PITCH    = 1,
            ROLL     = 2,
            YAW      = 3,
            THROTTLE = 4,
            HAT      = 5,

            BTN_PRESS   = 1 << 16,
            BTN_RELEASE = 1 << 17,
        
            INVALID
        };
    };
}


class idk::Joystick: public idk::DeviceController,
                     public idk::EventEmitter<uint32_t>
{
private:

public:
    Joystick();

    virtual bool canListen( idk::Device* ) final;

    union axes_type
    {
        float data[4] = {0, 0, 0, 0};
        struct { float roll, pitch, yaw, throttle; };
    };

    struct bounds_type
    {
        axes_type min, max;
    };

    axes_type   stick;
    bounds_type bounds;
    uint8_t     hat[8];

    // void clear()
    // {
    //     for (int i=0; i<sizeof(hat); i++)
    //     {
    //         hat[i] = SDL_HAT_CENTERED;
    //     }
    //     _changed = false;
    // }

    virtual void updateAxis( uint8_t axis, int16_t value ) override;

    void updateHat( uint8_t idx, uint8_t value );
};
