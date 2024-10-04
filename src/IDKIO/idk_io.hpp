#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "../IDKEvents/idk_keylog.hpp"


namespace idk::IO
{
    enum MouseButton { LMOUSE=0, MMOUSE=1, RMOUSE=2 };
    enum WinEvent    { WIN_EXIT=0, WIN_RESIZE=1 };

    void update( float dt );
    void setViewportOffset( int x, int y );

    void onPollEvent( std::function<void(SDL_Event*)> );
    bool windowEvent( uint32_t event );

    int  onMouseClick( uint32_t mouse, const std::function<void(int, int)> &callback );
    void removeMouseClickCallback( uint32_t mouse, int id );

    int  onMouseDrag( uint32_t mouse, const std::function<void(int, int)> &callback );
    void removeMouseDragCallback( uint32_t mouse, int id );


    void mouseCapture( bool );
    bool mouseCaptured();
    glm::vec2 mousePosition();
    glm::vec2 mouseDelta();

    bool  mouseUp( uint32_t );
    bool  mouseDown( uint32_t );
    bool  mouseClicked( uint32_t );
    float mouseWheelDelta();

    bool keyDown   ( idk::Keycode );
    bool keyUp     ( idk::Keycode );
    bool keyTapped ( idk::Keycode );

}

namespace idkio = idk::IO;



