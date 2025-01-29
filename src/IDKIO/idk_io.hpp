#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "../IDKEvents/idk_keylog.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <array>
#include <libidk/idk_wallocator.hpp>


namespace idk
{
    class IO;
    // inline static idk::IO *idkio;
}


class idk::IO
{
public:
    inline static idk::IO *global = nullptr;

private:
    bool m_Mcaptured   = false;

    glm::vec2  m_Moff   = glm::vec2(0.0f);
    glm::vec2  m_Mpos   = glm::vec2(0.0f);
    glm::vec2  m_Mdelta = glm::vec2(0.0f);

    std::array<bool, 3> m_mousebutton_up;
    std::array<bool, 3> m_mousebutton_down;
    std::array<bool, 3> m_mousebutton_clicked;
    float               m_mousewheel_delta = 0.0f;

    std::function<void(SDL_Event*)> m_poll_callback = [](SDL_Event*){};

    std::array<bool, 2>    m_windowevents     = { false, false };
    std::function<void()>  m_win_callbacks[2] = { [](){}, [](){} };

    idk::WAllocator<std::function<void(int, int)>> m_mousedrag_callbacks[3];
    idk::WAllocator<std::function<void(int, int)>> m_mouseclick_callbacks[3];
    idk::WAllocator<std::function<void(uint8_t, float)>> m_jaxis_callbacks;
    idk::WAllocator<std::function<void(uint8_t)>> m_jbtn_callbacks;

    idk::Keylog m_keylog;

    void processEvent( SDL_Event& );
    void pollEvents();


public:
    float    m_joystick_jaxis[8];
    uint8_t  m_joystick_jhat[8];
    uint8_t *m_joystick_btndown;

    enum MouseButton { LMOUSE=0, MMOUSE=1, RMOUSE=2 };
    enum WinEvent    { WIN_EXIT=0, WIN_RESIZE=1 };

    IO();

    void update( uint64_t msElapsed );
    void setViewportOffset( int x, int y );

    void onPollEvent( std::function<void(SDL_Event*)> );
    bool windowEvent( uint32_t event );

    int  onMouseClick( uint32_t mouse, const std::function<void(int, int)> &callback );
    void removeMouseClickCallback( uint32_t mouse, int id );

    int  onMouseDrag( uint32_t mouse, const std::function<void(int, int)> &callback );
    void removeMouseDragCallback( uint32_t mouse, int id );

    int  joystickAxisCreateCallback( const std::function<void(uint8_t, float)>& );
    void JoystickAxisRemoveCallback( int );
    int  joystickButtonCreateCallback( const std::function<void(uint8_t)>& );
    void JoystickButtonRemoveCallback( int );


    void mouseCapture( bool );
    void mouseReCapture();
    void mouseUnCapture();
    bool mouseCaptured();
    glm::vec2 mousePosition();
    glm::vec2 mouseDelta();

    bool  mouseUp( uint32_t );
    bool  mouseDown( uint32_t );
    bool  mouseClicked( uint32_t );
    float mouseWheelDelta();

    bool keyDown     ( idk::Keycode );
    bool keyUp       ( idk::Keycode );
    bool keyPressed  ( idk::Keycode );
    bool keyReleased ( idk::Keycode );
    bool keyTapped   ( idk::Keycode );
};



