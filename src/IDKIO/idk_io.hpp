#pragma once

#include <glm/glm.hpp>
#include "../IDKEvents/idk_keylog.hpp"
#include "./gamepad.hpp"

#include <libidk/idk_wallocator.hpp>

#include <SDL2/SDL.h>
#include <vector>
#include <array>
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
    inline static idk::IO *global = nullptr;

    enum InputMask: uint32_t
    {
        MOUSE_LEFT    = 1 << 16,
        MOUSE_MID     = 1 << 17,
        MOUSE_RIGHT   = 1 << 18,
        MOUSE_MOTION  = 1 << 19,
        MOUSE_WHEEL   = 1 << 20,
        KEY_PRESS     = 1 << 21,
        JSTICK_AXIS   = 1 << 22,
        JSTICK_BTN    = 1 << 23,
        GPAD_DOWN     = 1 << 24,
        GPAD_UP       = 1 << 25,
        GPAD_TAP      = 1 << 26,
        FILE_DROP     = 1 << 27
    };

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
    std::function<void(SDL_Event*)> m_poll_callback = [](SDL_Event*){};
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
        idk::WAllocator<std::function<void()>>               mouseclick[3];
        idk::WAllocator<std::function<void(float, float)>>   mousemotion;
        idk::WAllocator<std::function<void(float, float)>>   mousewheel;
        idk::WAllocator<std::function<void(idk::Keycode)>>   keypress;

        idk::WAllocator<std::function<void(uint8_t, float)>> joystickaxis;
        idk::WAllocator<std::function<void(uint8_t)>>        joystickbtn;
        idk::WAllocator<std::function<void(uint32_t)>>       gpadbtn[3];
        
        using callback_type = std::function<void(const std::string&)>;
        std::unordered_map<std::string, idk::WAllocator<callback_type>> dropfile;
    } m_callbacks;

    idk::Gamepad m_gpad;
    idk::Keylog m_keylog;

    void processEventGamepad( SDL_Event& );
    void processEvent( SDL_Event& );
    void pollEvents();


public:
    float    m_joystick_jaxis[8];
    uint8_t  m_joystick_jhat[8];
    uint8_t *m_joystick_btndown;

    // enum MouseButton: uint32_t { LMOUSE=0, MMOUSE=1, RMOUSE=2 };
    enum WinEvent: uint32_t    { WIN_EXIT=0, WIN_RESIZE=1 };

    IO();

    void update( uint64_t msElapsed );
    void setViewportOffset( int x, int y );

    void onPollEvent( std::function<void(SDL_Event*)> );
    bool windowEvent( uint32_t event );

    idk::Gamepad &getGamepad() { return m_gpad; }

    int onMouseClick( InputMask btn, const std::function<void()>& );
    int onMouseMotion( const std::function<void(float, float)>& );
    int onMouseWheel( const std::function<void(float, float)>& );
    int onKey( const std::function<void(idk::Keycode)>& );
    int onJoystickAxis( const std::function<void(uint8_t, float)>& );
    int onJoystickButton( const std::function<void(uint8_t)>& );
    int onGamepadButton( InputMask, const std::function<void(uint32_t)>& );
    int onFileDrop( const std::string &ext, const std::function<void(const std::string&)>& );
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

    bool gamepadButtonUp( Gamepad::Button );
    bool gamepadButtonDown( Gamepad::Button );
    bool gamepadButtonTapped( Gamepad::Button );


    bool keyDown     ( idk::Keycode );
    bool keyUp       ( idk::Keycode );
    bool keyPressed  ( idk::Keycode );
    bool keyReleased ( idk::Keycode );
    bool keyTapped   ( idk::Keycode );
};



