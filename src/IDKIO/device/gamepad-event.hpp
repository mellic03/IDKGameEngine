#pragma once
#include <SDL2/SDL.h>

namespace idk
{
    namespace internal
    {
        struct Gamepad_Event {
            enum type: int32_t {
                INVALID = SDL_CONTROLLER_BUTTON_INVALID,
                BTN_A,
                BTN_B,
                BTN_X,
                BTN_Y,
                BTN_BACK,
                BTN_GUIDE,
                BTN_START,
                BTN_LSTICK,
                BTN_RTSTICK,
                BTN_LSHOULDER,
                BTN_RSHOULDER,
                BTN_DPAD_UP,
                BTN_DPAD_DOWN,
                BTN_DPAD_LEFT,
                BTN_DPAD_RIGHT,
                BTN_MISC1,
                BTN_PADDLE1,
                BTN_PADDLE2,
                BTN_PADDLE3,
                BTN_PADDLE4,
                BTN_TOUCHPAD,

                LEFT_STICK,
                RIGHT_STICK,
                LEFT_TRIGGER,
                RIGHT_TRIGGER, // 24
            
                BTN_DWN  = 1 << 5, // 32
                BTN_UP   = 1 << 6,
                BTN_TAP  = 1 << 7,
            
                BTN_MAX
            };
        };
    }
}
