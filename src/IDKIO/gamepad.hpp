#pragma once
#include <SDL2/SDL.h>
#include <array>


namespace idk
{
    class Gamepad;
}


class idk::Gamepad
{
public:
    enum Button: uint32_t
    {
        BTN_A          = SDL_CONTROLLER_BUTTON_A,
        BTN_B          = SDL_CONTROLLER_BUTTON_B,
        BTN_X          = SDL_CONTROLLER_BUTTON_X,
        BTN_Y          = SDL_CONTROLLER_BUTTON_Y,
        BTN_BACK       = SDL_CONTROLLER_BUTTON_BACK,
        BTN_GUIDE      = SDL_CONTROLLER_BUTTON_GUIDE,
        BTN_START      = SDL_CONTROLLER_BUTTON_START,
        BTN_LSTICK     = SDL_CONTROLLER_BUTTON_LEFTSTICK,
        BTN_RTSTICK    = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
        BTN_LSHOULDER  = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        BTN_RSHOULDER  = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        BTN_DPAD_UP    = SDL_CONTROLLER_BUTTON_DPAD_UP,
        BTN_DPAD_DOWN  = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        BTN_DPAD_LEFT  = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        BTN_DPAD_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        BTN_MISC1      = SDL_CONTROLLER_BUTTON_MISC1,
        BTN_PADDLE1    = SDL_CONTROLLER_BUTTON_PADDLE1,
        BTN_PADDLE2    = SDL_CONTROLLER_BUTTON_PADDLE2,
        BTN_PADDLE3    = SDL_CONTROLLER_BUTTON_PADDLE3,
        BTN_PADDLE4    = SDL_CONTROLLER_BUTTON_PADDLE4,
        BTN_TOUCHPAD   = SDL_CONTROLLER_BUTTON_TOUCHPAD,
        BTN_MAX        = SDL_CONTROLLER_BUTTON_MAX
    };


private:
    void _clear();
    int  _getDataIdx( uint32_t axis );
    int  _getBoundsIdx( uint32_t axis );
    int  _getDeadzoneIdx( uint32_t axis );

public:
    SDL_GameController *ctl = nullptr;
    SDL_JoystickID      id;

    union {
        float data[24];
        struct {
            struct {
                float x, y, min, max, innerDZ, outerDZ;
            } lstick, rstick, ltrigger, rtrigger;
        };
    };

    std::array<bool, SDL_CONTROLLER_BUTTON_MAX> down;
    std::array<bool, SDL_CONTROLLER_BUTTON_MAX> tapped;
    std::array<uint32_t, SDL_CONTROLLER_BUTTON_MAX> timers;

    Gamepad();

    void setAxis( uint32_t axis, int16_t value );
    void setBtn( uint32_t btn, bool is_down );

    bool isOpen();
    void open( int index );
    void close();

};