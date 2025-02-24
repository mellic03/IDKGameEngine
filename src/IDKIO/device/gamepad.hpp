#pragma once
#include "gamepad-event.hpp"
#include "controller.hpp"
#include <array>
#include <libidk/idk_eventemitter.hpp>


namespace idk
{
    class Gamepad;
    using GamepadEvent = internal::Gamepad_Event::type;
}


class idk::Gamepad: public idk::DeviceController,
                    public idk::EventEmitterIndexed<uint32_t, void, GamepadEvent::BTN_MAX>
{
protected:
    void _clear();
    int  _getDataIdx( uint32_t axis );
    int  _getBoundsIdx( uint32_t axis );
    int  _getDeadzoneIdx( uint32_t axis );

    std::array<bool, SDL_CONTROLLER_BUTTON_MAX> _down;
    std::array<bool, SDL_CONTROLLER_BUTTON_MAX> _tapped;
    std::array<uint32_t, SDL_CONTROLLER_BUTTON_MAX> _timers;

public:
    union {
        float data[24];
        struct {
            struct {
                float x, y, min, max, innerDZ, outerDZ;
            } lstick, rstick, ltrigger, rtrigger;
        };
    };

    Gamepad();
    virtual ~Gamepad() = default;

    virtual void updateAxis( uint8_t axis, int16_t value ) override;
    virtual void updateButton( uint32_t btn, bool is_down ) override;
    virtual void update( uint32_t msElapsed ) override;

    bool buttonDown( uint32_t btn );
    bool buttonTapped( uint32_t btn );
};