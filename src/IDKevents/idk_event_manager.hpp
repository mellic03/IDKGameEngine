#pragma once

#include <functional>
#include "idk_keylog.hpp"

#include <libidk/idk_export.hpp>
#include <libidk/idk_enums.hpp>
#include <libidk/idk_glm.hpp>
#include <libidk/idk_allocator.hpp>
#include <libidk/idk_singleton.hpp>


namespace idk
{
    struct  Event;
    class   EventManager;

    enum class RenderEvent: int
    {
        LIGHTSOURCE_CHANGE
    };

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



struct IDK_VISIBLE idk::Event
{
    std::function<bool()>   trigger;
    std::function<void()>   response;
};



class IDK_VISIBLE idk::EventManager
{
    using fun_t = std::function<void(SDL_Event *)>;
    using mousefun_t = std::function<void(float f)>;

private:
    
    // SDL backend ------------------------------------
    std::vector<fun_t>      m_SDL_pollevents;
    // ------------------------------------------------

    Allocator<idk::Event>   m_events;
    std::vector<mousefun_t> m_scroll_events;
    bool                    m_windowevents[2];

    std::function<void(const char *)> _dropfile_callback;
    std::string                       m_dropfile_path  = "";
    bool                              m_dropfile_event = false;

    glm::ivec2              m_size;
    idk::Keylog             m_keylog;

    bool                    m_mouse_captured = false;
    std::vector<bool>       m_mousebutton_up;
    std::vector<bool>       m_mousebutton_down;
    float                   m_mousewheel_delta = 0.0f;
    glm::vec2               m_mouse_position;
    glm::vec2               m_mouse_delta;


public:
                            EventManager();
    Allocator<Event> &      events() { return m_events; };

    void                    processKeyInput();
    void                    processMouseInput();

    idk::Keylog &           keylog() { return m_keylog; };
    glm::ivec2              windowSize() const { return m_size; };
    
    void                    onWindowEvent( WindowEvent, std::function<void()> );

    void                    onDropFile( std::function<void(const char *)> callback );
    bool                    fileDropped() { return m_dropfile_event; };
    const std::string &     fileDroppedPath() { return m_dropfile_path; };

    void                    onSDLPollEvent( std::function<void(SDL_Event *)> fn) { m_SDL_pollevents.push_back(fn); };
    void                    onKeyEvent( idk::Keycode keycode, idk::KeyEvent keyevent, std::function<void()> callback );
    void                    onMouseWheel( std::function<void(float f)> callback );

    void                    mouseCapture( bool capture );
    bool                    mouseCaptured() const            { return m_mouse_captured;    };
    bool                    mouseUp( idk::MouseButton mb )   { return m_mousebutton_up[(int)mb];   };
    bool                    mouseDown( idk::MouseButton mb ) { return m_mousebutton_down[(int)mb]; };
    glm::vec2               mousePosition() { return m_mouse_position; };
    glm::vec2               mouseDelta()    { return m_mouse_delta;    };

    void                    update();

};


