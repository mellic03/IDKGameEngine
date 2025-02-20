#include "idk_io.hpp"
#include <libidk/idk_log2.hpp>
#include <libidk/idk_assert.hpp>
#include <filesystem>
using namespace idk;


void
IO::onPollEvent( std::function<void(SDL_Event*)> callback )
{
    m_poll_callback = callback;
}


// void
// IO::onWindowEvent( uint32_t event, std::function<void()> callback )
// {
//     event = (event % 2);
//     m_win_callbacks[event] = callback;
// }


bool
IO::windowEvent( uint32_t event )
{
    return m_windowevents[event];
}



void
IO::pollEvents()
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        processEvent(e);
    }
}



idk::IO::IO()
{
    idk::IO::global = this;

    m_joystick_btndown = new uint8_t[256];
    for (int i=0; i<256; i++)
    {
        m_joystick_btndown[i] = 0;
    }

    // SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS,"1");
    // int joysticks = SDL_Init(SDL_INIT_JOYSTICK);
    // LOG_INFO() << "[idk::IO::IO] " << joysticks << " joysticks available to read";

    // if (joysticks == 0)
    {
        if (SDL_JoystickOpen(0) == NULL)
        {
            LOG_WARN("idk::IO::IO", "Unable to read joystick");
        }
    }
}


void
idk::IO::update( uint64_t msElapsed )
{
    idk::IO::global = this;

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    m_keylog.update(state, msElapsed);

    int joysticks = SDL_NumJoysticks();
    for (int i=0; i<8; i++)
    {
        m_joystick_jaxis[i] = 0.0f;
        m_joystick_jhat[i] = 0;
    }

    {
        int x, y;

        SDL_GetMouseState(&x, &y);
        m_mouse.pos = glm::vec2(x+0.5f, y+0.5f) - m_mouse.offset;

        SDL_GetRelativeMouseState(&x, &y);
        m_mouse.delta = glm::vec2(x, y);

        m_mouse.captured = (SDL_GetRelativeMouseMode() == SDL_TRUE);
    }

    {
        m_windowevents[IO::WIN_EXIT]   = false;
        m_windowevents[IO::WIN_RESIZE] = false;

        m_mouse.wheel_delta = 0.0f;
        m_mouse.clicked[0] = false;
        m_mouse.clicked[1] = false;
        m_mouse.clicked[2] = false;

        pollEvents();
    }

    for (int i=0; i<m_gpad.down.size(); i++)
    {
        if (m_gpad.down[i])
            m_gpad.timers[i] += uint32_t(msElapsed);
    }

    if (fabs(m_mouse.delta.x) > 0.0f || fabs(m_mouse.delta.y) > 0.0f)
    {
        for (uint32_t i=0; i<3; i++)
        {
            if (m_mouse.clicked[i])
            {
                for (auto &[id, callback]: m_callbacks.mouseclick[i])
                {
                    callback();
                }
            }
        }
    }
}


int
maskToIdx( idk::IO::InputMask mask )
{
    int idx;

    switch (mask)
    {
        using enum idk::IO::InputMask;

        default:           idx=-1; break;

        case MOUSE_LEFT:   idx=0;  break;
        case MOUSE_MID:    idx=1;  break;
        case MOUSE_RIGHT:  idx=2;  break;
    
        case GPAD_DOWN:    idx=0;  break;
        case GPAD_UP:      idx=1;  break;
        case GPAD_TAP:     idx=2;  break;
    }

    if (idx == -1)
    {
        auto msg = std::format("Invalid InputMask {}", uint32_t(mask));
        IDK_ASSERT(msg.c_str(), false);
    }

    return idx;
}


int IO::onMouseClick( InputMask mask, const std::function<void()> &fn )
{
    int idx = maskToIdx(mask);
    return m_callbacks.mouseclick[idx].create(fn) | mask;
}

int IO::onMouseMotion( const std::function<void(float, float)> &fn )
{
    return m_callbacks.mousemotion.create(fn) | MOUSE_MOTION;
}

int IO::onMouseWheel( const std::function<void(float, float)> &fn )
{
    return m_callbacks.mousewheel.create(fn) | MOUSE_WHEEL;
}

int IO::onKey( const std::function<void(idk::Keycode)> &fn )
{
    return m_callbacks.keypress.create(fn) | KEY_PRESS;
}

int IO::onJoystickAxis( const std::function<void(uint8_t, float)> &fn )
{
    return m_callbacks.joystickaxis.create(fn) | JSTICK_AXIS;
}

int IO::onJoystickButton( const std::function<void(uint8_t)> &fn )
{
    return m_callbacks.joystickbtn.create(fn) | JSTICK_BTN;
}

int IO::onGamepadButton( InputMask mask, const std::function<void(uint32_t)> &fn )
{
    int idx = maskToIdx(mask);
    return m_callbacks.gpadbtn[idx].create(fn);
}

int IO::onFileDrop( const std::string &ext, const std::function<void(const std::string&)> &fn )
{
    return m_callbacks.dropfile[ext].create(fn) | FILE_DROP;
}


template <typename T>
static void remove_callback( int id, uint32_t mask, T &allocator )
{
    id &= ~(mask);
    allocator.destroy(id);
}


void
idk::IO::removeCallback( int callback_id )
{
    uint32_t id = uint32_t(callback_id);

    if      (id & MOUSE_LEFT)    remove_callback(id, MOUSE_LEFT,    m_callbacks.mouseclick[0]);
    else if (id & MOUSE_MID)     remove_callback(id, MOUSE_MID,     m_callbacks.mouseclick[1]);
    else if (id & MOUSE_RIGHT)   remove_callback(id, MOUSE_RIGHT,   m_callbacks.mouseclick[2]);
    else if (id & MOUSE_MOTION)  remove_callback(id, MOUSE_MOTION,  m_callbacks.mousemotion);
    else if (id & MOUSE_WHEEL)   remove_callback(id, MOUSE_WHEEL,   m_callbacks.mousemotion);

    else if (id & KEY_PRESS)     remove_callback(id, KEY_PRESS,     m_callbacks.keypress);

    else if (id & JSTICK_AXIS)   remove_callback(id, JSTICK_AXIS,   m_callbacks.joystickaxis);
    else if (id & JSTICK_BTN)    remove_callback(id, JSTICK_BTN,    m_callbacks.joystickbtn);

    else if (id & GPAD_DOWN)     remove_callback(id, GPAD_DOWN,     m_callbacks.gpadbtn[0]);
    else if (id & GPAD_UP)       remove_callback(id, GPAD_UP,       m_callbacks.gpadbtn[1]);
    else if (id & GPAD_TAP)      remove_callback(id, GPAD_TAP,      m_callbacks.gpadbtn[2]);

    else if (id & FILE_DROP)     remove_callback(id, FILE_DROP,     m_callbacks.joystickbtn);

    else    LOG_ERROR("idk::IO::removeCallback", std::format("Unknown id {}", id));
}



void      IO::setViewportOffset( int x, int y ) { m_mouse.offset = glm::vec2(x, y); }

void      IO::mouseCapture( bool b ) { SDL_SetRelativeMouseMode((b) ? SDL_TRUE : SDL_FALSE); }
void      IO::mouseRelease(        ) { SDL_SetRelativeMouseMode(SDL_FALSE); }
bool      IO::mouseCaptured() { return m_mouse.captured; }
glm::vec2 IO::mousePosition() { return m_mouse.pos;      }
glm::vec2 IO::mouseDelta()    { return m_mouse.delta;    }

bool      IO::mouseUp         ( uint32_t i )  { return m_mouse.up[i];       }
bool      IO::mouseDown       ( uint32_t i )  { return m_mouse.down[i];     }
bool      IO::mouseClicked    ( uint32_t i )  { return m_mouse.clicked[i];  }
float     IO::mouseWheelDelta (            )  { return m_mouse.wheel_delta; }


bool IO::gamepadButtonUp( Gamepad::Button b )
{
    return !gamepadButtonDown(b);
}

bool IO::gamepadButtonDown( Gamepad::Button b )
{
    return m_gpad.down[static_cast<uint32_t>(b)];
}

bool IO::gamepadButtonTapped( Gamepad::Button b )
{
    return m_gpad.tapped[static_cast<uint32_t>(b)];
}





bool      IO::keyDown     ( idk::Keycode key ) { return m_keylog.keyDown(key);     }
bool      IO::keyUp       ( idk::Keycode key ) { return m_keylog.keyUp(key);       }
bool      IO::keyReleased ( idk::Keycode key ) { return m_keylog.keyReleased(key); }
bool      IO::keyPressed  ( idk::Keycode key ) { return m_keylog.keyPressed(key);  }
bool      IO::keyTapped   ( idk::Keycode key ) { return m_keylog.keyTapped(key);   }





// void
// idk::EventSystem::processMouseInput()
// {
//     // Reset window events
//     for (bool &b: m_windowevents)
//     {
//         b = false;
//     }

//     m_mouse.wheel_delta = 0.0f;
//     m_dropfile_event = false;

//     m_mouse.clicked[0] = false;
//     m_mouse.clicked[1] = false;
//     m_mouse.clicked[2] = false;

//     SDL_Event e;

//     while (SDL_PollEvent(&e))
//     {
//         switch (e.type)
//         {
//             case SDL_QUIT:
//                 m_windowevents[(int)(WindowEvent::EXIT)] = true;
//             break;


//             case SDL_DROPFILE:
//                 // m_dropfile_path  = e.drop.file;
//             break;


//             case SDL_WINDOWEVENT:
//                 // if (e.window.event == SDL_WINDOWEVENT_CLOSE)
//                 // {
//                 //     m_windowevents[(int)(WindowEvent::EXIT)] = true;
//                 // }
//                 // else if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
//                 // {
//                 //     m_windowevents[(int)(WindowEvent::RESIZE)] = true;
//                 //     m_size.x = e.window.data1;
//                 //     m_size.y = e.window.data2;
//                 // }
//             break;


//             case (SDL_MOUSEBUTTONDOWN):
//             break;


//             case (SDL_MOUSEBUTTONUP):
//             break;


//             case (SDL_MOUSEWHEEL):
//                 // m_mouse.wheel_delta = e.wheel.y;
//             break;
//         }

//         if (m_SDL_pollevents.size() > 0)
//         {
//             m_SDL_pollevents.back()(&e);
//         }
//     }

//     int x, y;

//     SDL_GetMouseState(&x, &y);
//     m_mouse_position.x = float(x);
//     m_mouse_position.y = float(y);

//     SDL_GetRelativeMouseState(&x, &y);
//     m_mouse_delta.x = float(x);
//     m_mouse_delta.y = float(y);
// }
