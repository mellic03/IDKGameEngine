#pragma once

#include <glm/glm.hpp>
#include "../IDKEvents/idk_keylog.hpp"
#include "./device/gamepad.hpp"
#include "./callback.hpp"

#include <libidk/idk_assert.hpp>
#include <libidk/idk_wallocator.hpp>
#include <libidk/idk_eventemitter.hpp>
#include <libidk/idk_memory.hpp>

#include <SDL2/SDL.h>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <functional>

namespace idk
{
    class IO;
}
using idkio = idk::IO;


class idk::IO
{
public:
    enum InputIdx: uint32_t
    {
        IDX_MOUSE_LEFT   = 0,
        IDX_MOUSE_MID    = 1,
        IDX_MOUSE_RIGHT  = 2,
        IDX_MOUSE_MOTION = 3,
        IDX_JSTICK_AXIS  = 4,
        IDX_JSTICK_BTN   = 5,

        IDX_GPAD_DOWN    = 0,
        IDX_GPAD_UP      = 1,
        IDX_GPAD_TAPPED  = 2
    };

private:
    std::array<bool, 2>    m_windowevents     = { false, false };
    std::function<void()>  m_win_callbacks[2] = { [](){}, [](){} };

    struct
    {
        bool captured     = false;
        bool up[3]        = { true, true, true };
        bool down[3]      = { false, false, false };
        bool clicked[3]   = { false, false, false };
        float wheel_delta = 0.0f;
    
        glm::vec2 pos    = glm::vec2(0);
        glm::vec2 delta  = glm::vec2(0);
        glm::vec2 offset = glm::vec2(0);
    } m_mouse;

    struct
    {
        idk::WAllocator<CallbackHandle>                       handles;
        idk::WAllocator<std::function<void(SDL_Event*)>>      pollEvents;

        idk::WAllocator<std::function<void()>>                mouseclick[3];
        idk::WAllocator<std::function<void(float, float)>>    mousemotion;
        idk::WAllocator<std::function<void(float, float)>>    mousewheel;
        idk::WAllocator<std::function<void(idk::Keycode)>>    keypress;

        using callback_type = std::function<void(const std::string&)>;
        std::unordered_map<std::string, idk::WAllocator<callback_type>> dropfile;
    } m_callbacks;

    idk::WAllocator<idk::DeviceController*> m_controllers;
    std::unordered_map<int, idk::Device*>   m_devices;

    // idk::Gamepad  m_gpad;
    idk::Keylog   m_keylog;

    void _processEventController( SDL_Event& );
    void _processEvent( SDL_Event& );
    void _processEvents();
    int  _createCallback( uint32_t type, int32_t idx );


public:
    // enum MouseButton: uint32_t { LMOUSE=0, MMOUSE=1, RMOUSE=2 };
    enum WinEvent: uint32_t { WIN_EXIT=0, WIN_RESIZE=1 };

    IO();

    void update( uint64_t msElapsed );
    void setViewportOffset( int x, int y );

    bool windowEvent( uint32_t event );

    int  onPollEvent( std::function<void(SDL_Event*)> );
    int  onMouseClick( CallbackMask btn, const std::function<void()>& );
    int  onMouseMotion( const std::function<void(float, float)>& );
    int  onMouseWheel( const std::function<void(float, float)>& );
    int  onKey( const std::function<void(idk::Keycode)>& );
    int  onFileDrop( const std::string &ext, const std::function<void(const std::string&)>& );
    void removeCallback( int );


    void mouseCapture( bool b = true);
    void mouseRelease();
    bool mouseCaptured();
    glm::vec2 mousePosition();
    glm::vec2 mouseDelta();

    bool  mouseUp( uint32_t );
    bool  mouseDown( uint32_t );
    bool  mouseClicked( uint32_t );
    float mouseWheelDelta();

    // bool gamepadButtonUp( Gamepad::Button );
    // bool gamepadButtonDown( Gamepad::Button );
    // bool gamepadButtonTapped( Gamepad::Button );

    bool keyDown     ( idk::Keycode );
    bool keyUp       ( idk::Keycode );
    bool keyPressed  ( idk::Keycode );
    bool keyReleased ( idk::Keycode );
    bool keyTapped   ( idk::Keycode );


    std::unordered_map<int, idk::Device*> &getDevices();
    idk::Device *getDevice( int which );
    void         createDevice( int which );
    void         destroyDevice( int which );

    idk::DeviceController *getController( int id );

    template <typename ctl_type>
    ctl_type *createController()
    {
        DeviceController *base = new ctl_type();

        ctl_type *ctl = dynamic_cast<ctl_type*>(base);
                  ctl->ID = m_controllers.create(base);
                  ctl->m_io = this;

        return ctl;
    }

    auto &getControllers()
    {
        return m_controllers;
    }

    void destroyController( idk::DeviceController *ctl )
    {
        auto *dev = ctl->dev;

        if (dev)
        {
            IDK_ASSERT("Something is very wrong here", dev->controllers.contains(ctl->ID));
            dev->controllers.erase(ctl->ID);
        }

        m_controllers.destroy(ctl->ID);
    }

};



