#pragma once

#include "IDKcommon/IDKcommon.h"


namespace idk
{
    struct  Event;
    class   EventManager;

    enum class WindowEvent: int
    {
        RESIZE,
        EXIT,
        NUM_EVENTS
    };

    enum class MouseEvent: int
    {
        MOTION,
        BUTTON,
        NUM_EVENTS
    };
};


struct idk::Event
{
    std::function<bool()>   trigger;
    std::function<void()>   response;
};


class idk::EventManager
{
private:
    
    // SDL backend ------------------------------------
    SDL_Event               m_SDL_Event;
    idk::vector<std::function<void(SDL_Event *)>> m_SDL_pollevents;
    // ------------------------------------------------

    Allocator<idk::Event>   m_events;
    idk::vector<bool>       m_windowevents;

    struct WindowData       { int width, height; };
    WindowData              m_window_data;

    idk::Keylog             m_keylog;

    bool                    m_mouse_captured = false;
    idk::vector<bool>       m_mousebutton_up;
    idk::vector<bool>       m_mousebutton_down;
    glm::vec2               m_mouse_position;
    glm::vec2               m_mouse_delta;


public:
                            EventManager();
    Allocator<Event> &      events() { return m_events; };

    void                    processKeyInput();
    void                    processMouseInput();

    idk::Keylog &           keylog() { return m_keylog; };
    const WindowData &      windowData() const { return m_window_data; };
    
    void                    onWindowEvent( WindowEvent, std::function<void()> );
    void                    onSDLPollEvent( std::function<void(SDL_Event *)> fn) { m_SDL_pollevents.push(fn); };

    void                    mouseCapture( bool capture );
    bool                    mouseCaptured() const       { return m_mouse_captured;    };
    bool                    mouseUp( idk::MouseButton mb )   { return m_mousebutton_up[(int)mb];   };
    bool                    mouseDown( idk::MouseButton mb ) { return m_mousebutton_down[(int)mb]; };
    glm::vec2               mousePosition() { return m_mouse_position; };
    glm::vec2               mouseDelta()    { return m_mouse_delta;    };

    void                    update();

};
