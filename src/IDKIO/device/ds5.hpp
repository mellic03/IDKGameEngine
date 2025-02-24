#pragma once
#include "gamepad.hpp"
#include "ds5-state.hpp"

namespace idk
{
    class DS5;

    using DS5Trigger  = internal::DS5_Trigger::type;
    using DS5Property = internal::DS5_Property::type;
    using DS5Value    = internal::DS5_Value::type;

    using DS5EnableBits1 = internal::DS5_EnableBits1::type;
    using DS5EnableBits2 = internal::DS5_EnableBits2::type;
}


class idk::DS5: public idk::Gamepad
{
private:
    bool _state_dirty = false;
    DS5State _state;

public:
    DS5();
    virtual void update( uint32_t msElapsed ) final;
    virtual bool canListen( idk::Device* ) final;

    // void setTriggerMode( DS5Trigger trigger, DS5TriggerMode mode );
    void setProperty( DS5Trigger trigger, DS5Property property, uint8_t value );
    void setProperty( DS5Trigger trigger, DS5Property property, float alpha );
};
