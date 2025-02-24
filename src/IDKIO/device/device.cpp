#include "device.hpp"
#include "controller.hpp"
#include "../idk_io.hpp"

#include <SDL2/SDL.h>
#include <libidk/idk_assert.hpp>
#include <libidk/idk_log.hpp>
#include <glm/glm.hpp>
using lf = idk::log_flag;


idk::Device::Device( idk::IO &io )
:   m_io(io)
{
    LOG_ADV(lf::IO|lf::DETAIL, "");
}


idk::Device::~Device()
{
    static std::vector<int> cull;
    cull.clear();

    for (int ID: this->controllers)
    {
        cull.push_back(ID);
    }

    for (int ID: cull)
    {
        this->removeListener(ID);
    }
}


void
idk::Device::addListener( DeviceController *ctl )
{
    ctl->dev = this;
    this->controllers.insert(ctl->ID);
    ctl->onListen();
    LOG_ADV(lf::IO|lf::DETAIL, "Added listening controller {}", ctl->ID);
}


void
idk::Device::removeListener( int ID )
{
    this->controllers.erase(ID);
    m_io.getController(ID)->dev = nullptr;
    LOG_ADV(lf::IO|lf::DETAIL, "Removed listening controller {}", ID);
}


void
idk::Device::updateAxis( uint8_t axis, int16_t value )
{
    // LOG_ADV(lf::IO|lf::DETAIL, "axis {} {}", int(axis), int(value));
    for (int ID: this->controllers)
    {
        m_io.getController(ID)->updateAxis(axis, value);
    }
}


void
idk::Device::updateButton( uint32_t btn, bool is_down )
{
    // LOG_ADV(lf::IO|lf::DETAIL, "button {} {}", btn, is_down);
    // LOG_ADV(lf::IO|lf::DETAIL, "No. controllers: {}", controllers.size());

    for (int ID: this->controllers)
    {
        m_io.getController(ID)->updateButton(btn, is_down);
    }
}


bool
idk::Device::isValid()
{
    return impl != nullptr && which != -1;
}

// bool
// idk::Device::isOpen()
// {
//     return impl != nullptr;
// }


// bool
// idk::Device::open( int index )
// {
//     impl  = SDL_GameControllerOpen(index);
//     deviceIdx  = index;
//     instanceID = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(impl));
//     return impl != nullptr;
// }


// void
// idk::Device::close()
// {
//     SDL_GameControllerClose(impl);

//     static std::vector<int> cull;
//     cull.clear();

//     for (int ID: this->controllers)
//     {
//         cull.push_back(ID);
//     }

//     for (int ID: cull)
//     {
//         this->removeListener(ID);
//     }

//     impl = nullptr;
// }


bool
idk::Device::getProductInfo( std::string *name, uint16_t *vendor, uint16_t *product,
                                 uint16_t *version, uint16_t *firmware )
{
    // if (this->isOpen() == false)
    // {
    //     return false;
    // }

    if (name)     *name     = SDL_GameControllerName(this->impl);
    if (vendor)   *vendor   = SDL_GameControllerGetVendor(this->impl);
    if (product)  *product  = SDL_GameControllerGetProduct(this->impl);
    if (version)  *version  = SDL_GameControllerGetProductVersion(this->impl);
    if (firmware) *firmware = SDL_GameControllerGetFirmwareVersion(this->impl);

    return true;
}
