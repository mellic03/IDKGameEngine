#pragma once

#include <cstddef>
#include <cstdint>


namespace idk
{
    struct CallbackType
    {
        enum enum_type: uint32_t
        {
            POLL_EVENTS = 1,
            MOUSE_LEFT,
            MOUSE_MID,
            MOUSE_RIGHT,
            MOUSE_MOTION,
            MOUSE_WHEEL,
            KEY_PRESS,
            JSTICK_CHANGE,
            JSTICK_BTN,
            GPAD_DOWN,
            GPAD_UP,
            GPAD_TAP,
            FILE_DROP
        };
    };
    using CallbackMask = CallbackType::enum_type;


    struct CallbackHandle
    { 
        uint32_t mask;
        int32_t  idx;
    };
}

