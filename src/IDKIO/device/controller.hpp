#pragma once
#include <vector>
#include <functional>

#include "device.hpp"


namespace idk
{
    class IO;
    class DeviceController;
}


class idk::DeviceController
{
private:
    friend class idk::IO;
    idk::IO *m_io;

protected:
    std::vector<uint8_t>  _down;
    std::vector<uint8_t>  _tapped;
    std::vector<uint32_t> _timer;
    std::vector<float>    _axes;

public:
    int          ID      = -1;
    idk::Device *dev     = nullptr;
    bool         changed = false;
    std::function<void()> onListen = [](){};

    DeviceController( uint32_t num_buttons=64, uint32_t num_axes=0 );
    virtual ~DeviceController();

    virtual void updateAxis( uint8_t axis, int16_t value );
    virtual void updateButton( uint32_t btn, bool is_down );
    virtual void update( uint32_t msElapsed );

    virtual bool buttonDown( uint32_t btn );
    virtual bool buttonTapped( uint32_t btn );

    bool isListening();
    void stopListening();
    virtual bool canListen( idk::Device* );
    // virtual void unbind();

};