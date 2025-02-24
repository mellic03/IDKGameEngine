#include "ds5.hpp"
#include <libidk/idk_assert.hpp>
#include <libidk/idk_log.hpp>
#include <cstring>
using lf = idk::log_flag;


idk::DS5::DS5()
:   idk::Gamepad()
{
    LOG_ADV(lf::IO|lf::DETAIL, "");

    std::memset(&_state, 0, sizeof(_state));
    // std::memset(_outReport, 0, sizeof(_outReport)/sizeof(_outReport[0]));
    // std::memset(_effects, 0, sizeof(_effects)/sizeof(uint8_t));

    uint8_t effect[11] = { 0x02, 50, 20, 75, 0, 0, 0, 0, 0, 0, 0 };

    for (int i=0; i<11; i++)
    {
        _state.RTriggerFFB[i] = effect[i];
        _state.LTriggerFFB[i] = effect[i];
    }
}


void
idk::DS5::update( uint32_t msElapsed )
{
    Gamepad::update(msElapsed);

    // DS5StateBitSet(_state.EnableBits1, DS5EnableBits1::)

    if (this->dev == nullptr)
    {
        return;
    }

    if (_state_dirty)
    {
        _state_dirty = false;
        _state.EnableBits1 = 0x04 | 0x08; // Modify right and left trigger effect respectively
        SDL_GameControllerSendEffect(this->dev->impl, &_state, sizeof(_state));
    }
}


void
idk::DS5::setProperty( DS5Trigger trigger, DS5Property idx, uint8_t value )
{
    _state_dirty = true;
    if (trigger | DS5Trigger::LEFT)  { _state.LTriggerFFB[idx] = value; }
    if (trigger | DS5Trigger::RIGHT) { _state.RTriggerFFB[idx] = value; }
}


void
idk::DS5::setProperty( DS5Trigger trigger, DS5Property idx, float alpha )
{
    setProperty(trigger, idx, uint8_t(255.0f * alpha));
}



bool
idk::DS5::canListen( idk::Device *device )
{
    if (SDL_GameControllerGetType(device->impl) != SDL_CONTROLLER_TYPE_PS5)
    {
        return false;
    }

    return DeviceController::canListen(device);
}
