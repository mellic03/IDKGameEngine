// #pragma once

// #include <libidk/idk_export.hpp>
// #include <libidk/idk_enums.hpp>
// #include <libidk/idk_glm.hpp>
// #include <libidk/idk_allocator.hpp>

// #include "idk_keylog.hpp"

// #include <functional>
// #include <set>
// #include <map>


// namespace idk
// {
//     struct  Event;
//     class   EventSystem;

//     enum class RenderEvent: int
//     {
//         LIGHTSOURCE_CHANGE
//     };

//     enum class WindowEvent: int
//     {
//         RESIZE,
//         EXIT,
//         NUM_EVENTS
//     };

//     enum class MouseEvent: int
//     {
//         MOTION,
//         BUTTON,
//         NUM_EVENTS
//     };
// };



// struct IDK_VISIBLE idk::Event
// {
//     std::function<bool()>   trigger;
//     std::function<void()>   response;
// };



// class IDK_VISIBLE idk::EventSystem
// {
//     using fun_t = std::function<void(SDL_Event *)>;
//     using mousefun_t = std::function<void(float f)>;

// private:

//     struct KeyEventWrapper
//     {
//         idk::Keycode keycode;
//         std::function<void()> callback;
//     };


//     // SDL backend ------------------------------------
//     std::vector<fun_t>      m_SDL_pollevents;
//     // ------------------------------------------------

//     Allocator<idk::Event>   m_events;
//     std::vector<mousefun_t> m_scroll_events;
//     bool                    m_windowevents[2];

//     std::map<std::string, bool> m_dropfile_set;
//     std::map<std::string, std::function<void(const char *)>> m_dropfile_callbacks;
//     std::string                       m_dropfile_path  = "";
//     bool                              m_dropfile_event = false;

//     glm::ivec2              m_size;
//     idk::Keylog             m_keylog;

//     bool                    m_mouse_captured = false;
//     std::vector<bool>       m_mousebutton_up;
//     std::vector<bool>       m_mousebutton_down;
//     std::vector<bool>       m_mousebutton_clicked;
//     float                   m_mousewheel_delta = 0.0f;
//     glm::vec2               m_mouse_offset = glm::vec2(0.0f);
//     glm::vec2               m_mouse_position;
//     glm::vec2               m_mouse_delta;


//     std::vector<KeyEventWrapper> m_KeyDown_events;
//     std::vector<KeyEventWrapper> m_KeyUp_events;
//     std::vector<KeyEventWrapper> m_KeyTapped_events;



// public:
//                             EventSystem();
//     Allocator<Event> &      events() { return m_events; };

//     void                    processKeyInput();
//     void                    processMouseInput();

//     idk::Keylog &           keylog() { return m_keylog; };
//     glm::ivec2              windowSize() const { return m_size; };
    
//     void                    onWindowEvent( WindowEvent, std::function<void()> );

//     void                    onDropFile( const std::string &extension, std::function<void(const char *)> callback );
//     bool                    fileDropped() { return m_dropfile_event; };
//     const std::string &     fileDroppedPath() { return m_dropfile_path; };

//     void                    onSDLPollEvent( std::function<void(SDL_Event *)> fn) { m_SDL_pollevents.push_back(fn); };

//     void                    onKeyEvent( idk::Keycode keycode, idk::KeyEvent keyevent, std::function<void()> callback );
//     void                    onMouseWheel( std::function<void(float f)> callback );

//     void                    onKeyDown   ( idk::Keycode, std::function<void()> );
//     void                    onKeyUp     ( idk::Keycode, std::function<void()> );
//     void                    onKeyTapped ( idk::Keycode, std::function<void()> );


//     void                    mouseCapture( bool capture );
//     bool                    mouseCaptured() const            { return m_mouse_captured;    };
//     bool                    mouseUp( idk::MouseButton mb )   { return m_mousebutton_up[(int)mb];   };
//     bool                    mouseDown( idk::MouseButton mb ) { return m_mousebutton_down[(int)mb]; };
//     bool                    mouseClicked( idk::MouseButton mb ) { return m_mousebutton_clicked[(int)mb]; };;

//     glm::vec2               mousePosition()   { return m_mouse_position - m_mouse_offset; };
//     glm::vec2               mouseDelta()      { return m_mouse_delta;    };
//     float                   mouseWheelDelta() { return m_mousewheel_delta; }

//     void                    setMouseOffset( const glm::vec2 &offset ) { m_mouse_offset = offset; };

//     void                    update();


// };


