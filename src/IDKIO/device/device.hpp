#pragma once
#include <cstdint>
#include <cstddef>
#include <string>
#include <set>

struct _SDL_GameController;

namespace idk
{
    class IO;

    class DeviceController;
    class Device;
}


class idk::Device
{
private:
    idk::IO &m_io;

public:
    Device( idk::IO& );
    ~Device();

    std::set<int>        controllers;
    _SDL_GameController *impl  = nullptr;
    int32_t              which = -1;

    void addListener( DeviceController* );
    void removeListener( int );
    void updateAxis( uint8_t axis, int16_t value );
    void updateButton( uint32_t btn, bool is_down );

    // bool isOpen();
    // bool open( int index );
    // void close();
    bool isValid();

    /**
     * @return true on success, false on failure.
     */
    bool getProductInfo( std::string *name, uint16_t *vendor, uint16_t *product,
                         uint16_t *productVersion, uint16_t *firmwareVersion );

};