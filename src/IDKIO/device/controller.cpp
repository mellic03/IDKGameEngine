#include "controller.hpp"
#include <libidk/idk_assert.hpp>
#include <libidk/idk_log.hpp>
#include <glm/glm.hpp>

#include <SDL2/SDL.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

using lf = idk::log_flag;


idk::DeviceController::DeviceController( uint32_t num_buttons, uint32_t num_axes )
:   _down   (num_buttons, 0),
    _tapped (num_buttons, 0),
    _timer  (num_buttons, 0),
    _axes   (num_axes, 0.0f)
{
    LOG_ADV(lf::IO|lf::DETAIL, "");

}

idk::DeviceController::~DeviceController()
{
    LOG_ADV(lf::IO|lf::DETAIL, "");
    this->stopListening();
}





void
idk::DeviceController::update( uint32_t msElapsed )
{
    for (int i=0; i<_down.size(); i++)
    {
        if (_down[i])
            _timer[i] += uint32_t(msElapsed);
    }
}


bool
idk::DeviceController::buttonDown( uint32_t btn )
{
    return _down[btn];
}


bool
idk::DeviceController::buttonTapped( uint32_t btn )
{
    return _tapped[btn];
}


void
idk::DeviceController::updateAxis( uint8_t axis, int16_t value )
{
    
}


void
idk::DeviceController::updateButton( uint32_t idx, bool is_down )
{
    bool was_down = _down[idx];
    bool is_up    = !is_down;

    _down[idx] = is_down;

    if (was_down && is_up)
    {
        _tapped[idx] = (_timer[idx] < 250);
    }

    if (is_down)
    {
        _tapped[idx] = false;
    }

    else
    {
        _timer[idx] = 0;
    }
}



bool
idk::DeviceController::isListening()
{
    return this->dev != nullptr;
}


void
idk::DeviceController::stopListening()
{
    if (this->isListening())
    {
        this->dev->removeListener(this->ID);
    }
}


bool
idk::DeviceController::canListen( idk::Device *device )
{
    return true;
}


// void
// idk::DeviceController::unbind()
// {
//     this->port = nullptr;
// }

