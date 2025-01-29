#include "idk_io.hpp"

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
IO::processEvent( SDL_Event &e )
{
    switch (e.type)
    {
        case SDL_QUIT:
            m_windowevents[IO::WIN_EXIT] = true;
            break;

        case SDL_DROPFILE:
        {
            // m_dropfile_event = true;
            // m_dropfile_path  = e.drop.file;
            // {
            //     std::string extension = fs::path(m_dropfile_path).extension();

            //     if (m_dropfile_set[extension])
            //     {
            //         m_dropfile_callbacks[extension](e.drop.file);
            //     }
            // }
            break;
        }

        // case SDL_KEYDOWN: std::cout << "[IO] SDL_KEYDOWN\n"; m_keylog.log(e.key.keysym.scancode, true);  break;
        // case SDL_KEYUP:   std::cout << "[IO] SDL_KEYUP\n"; m_keylog.log(e.key.keysym.scancode, false); break;

        case SDL_WINDOWEVENT:
        {
            if (e.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                m_windowevents[IO::WIN_EXIT] = true;
            }

            else if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                m_windowevents[IO::WIN_RESIZE] = true;
            }
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            // Good for callbacks?
            // x = e.button.x;
            // y = e.button.y;

            m_mousebutton_down[e.button.button - 1] = true;
            m_mousebutton_up[e.button.button - 1] = false;
            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            bool prev_down = m_mousebutton_down[e.button.button - 1];
            bool prev_up   = m_mousebutton_up[e.button.button - 1];

            m_mousebutton_down[e.button.button - 1] = false;
            m_mousebutton_up[e.button.button - 1] = true;

            bool curr_down = m_mousebutton_down[e.button.button - 1];
            bool curr_up   = m_mousebutton_up[e.button.button - 1];

            m_mousebutton_clicked[e.button.button-1] = (prev_down == true && curr_down == false);
            break;
        }

        case SDL_MOUSEWHEEL:
        {
            m_mousewheel_delta = e.wheel.y;
            break;
        }

        case SDL_JOYAXISMOTION:
            m_joystick_jaxis[e.jaxis.axis] = e.jaxis.value;

            for (auto &[id, callback]: m_jaxis_callbacks)
            {
                callback(uint8_t(e.jaxis.axis), e.jaxis.value);
            }
            // std::cout << "[IO::processEvent] SDL_JOYAXISMOTION " << int(e.jaxis.axis) << ": " << e.jaxis.value << "\n";
            break;

        case SDL_JOYHATMOTION:
            m_joystick_jhat[e.jhat.hat] = e.jhat.value; 
            std::cout << "[IO::processEvent] SDL_JOYHATMOTION" << int(e.jhat.hat) << ": " << int(e.jhat.value) << "\n";
            break;

        case SDL_JOYBUTTONDOWN:
            m_joystick_btndown[e.jbutton.button] = true;
            // std::cout << "[IO::processEvent] SDL_JOYBUTTONDOWN" << int(e.jbutton.button) << "\n";
            break;

        case SDL_JOYBUTTONUP:
            m_joystick_btndown[e.jbutton.button] = false;
            // std::cout << "[IO::processEvent] SDL_JOYBUTTONDOWN" << int(e.jbutton.button) << "\n";
            break;

        default:
            break;
    }

    m_poll_callback(&e);
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

    int joysticks = SDL_Init(SDL_INIT_JOYSTICK);
    LOG_INFO() << "[idk::IO::IO] " << joysticks << " joysticks available to read";

    if (joysticks == 0)
    {
        if (SDL_JoystickOpen(0) == NULL)
        {
            LOG_ERROR() << "[idk::IO::IO] Unable to read joystick";
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
        m_Mpos = glm::vec2(x+0.5f, y+0.5f) - m_Moff;

        SDL_GetRelativeMouseState(&x, &y);
        m_Mdelta = glm::vec2(x, y);

        m_Mcaptured = (SDL_GetRelativeMouseMode() == SDL_TRUE);
    }

    {
        m_windowevents[IO::WIN_EXIT]   = false;
        m_windowevents[IO::WIN_RESIZE] = false;

        m_mousewheel_delta = 0.0f;
        m_mousebutton_clicked[0] = false;
        m_mousebutton_clicked[1] = false;
        m_mousebutton_clicked[2] = false;

        pollEvents();
    }

    if (fabs(m_Mdelta.x) > 0.0f || fabs(m_Mdelta.y) > 0.0f)
    {
        for (uint32_t i=0; i<3; i++)
        {
            if (mouseDown(i))
            {
                for (auto &[id, callback]: m_mousedrag_callbacks[i])
                {
                    callback(m_Mdelta.x, m_Mdelta.y);
                }
            }

            if (mouseClicked(i))
            {
                for (auto &[id, callback]: m_mouseclick_callbacks[i])
                {
                    callback(m_Mdelta.x, m_Mdelta.y);
                }
            }
        }
    }

    // for (int i=0; i<2; i++)
    // {
    //     if (m_windowevents[i] == true)
    //     {
    //         m_win_callbacks[i]();
    //     }
    // }
}



int
IO::onMouseClick( uint32_t mouse, const std::function<void(int, int)> &callback )
{
    return m_mouseclick_callbacks[mouse].create(callback);
}


void
IO::removeMouseClickCallback( uint32_t mouse, int id )
{
    m_mouseclick_callbacks[mouse].destroy(id);
}



int
IO::onMouseDrag( uint32_t mouse, const std::function<void(int, int)> &callback )
{
    return m_mousedrag_callbacks[mouse].create(callback);
}


void
IO::removeMouseDragCallback( uint32_t mouse, int id )
{
    m_mousedrag_callbacks[mouse].destroy(id);
}



int
idk::IO::joystickAxisCreateCallback( const std::function<void(uint8_t, float)> &fn )
{
    return m_jaxis_callbacks.create(fn);
}

void
idk::IO::JoystickAxisRemoveCallback( int id )
{
    m_jaxis_callbacks.destroy(id);
}


int
idk::IO::joystickButtonCreateCallback( const std::function<void(uint8_t)> &fn )
{
    return m_jbtn_callbacks.create(fn);
}

void
idk::IO::JoystickButtonRemoveCallback( int id )
{
    m_jbtn_callbacks.destroy(id);
}




void      IO::setViewportOffset( int x, int y ) { m_Moff = glm::vec2(x, y); }
void      IO::mouseCapture( bool b ) { SDL_SetRelativeMouseMode((b) ? SDL_TRUE : SDL_FALSE); }


void      IO::mouseReCapture() { SDL_SetRelativeMouseMode(SDL_TRUE); }
void      IO::mouseUnCapture() { SDL_SetRelativeMouseMode(SDL_FALSE); }
bool      IO::mouseCaptured()        { return m_Mcaptured;     }
glm::vec2 IO::mousePosition()        { return m_Mpos; }
glm::vec2 IO::mouseDelta()           { return m_Mdelta;        }

bool      IO::mouseUp         ( uint32_t b )  { return m_mousebutton_up[b];      }
bool      IO::mouseDown       ( uint32_t b )  { return m_mousebutton_down[b];    }
bool      IO::mouseClicked    ( uint32_t b )  { return m_mousebutton_clicked[b]; }
float     IO::mouseWheelDelta (            )  { return m_mousewheel_delta;       }



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

//     m_mousewheel_delta = 0.0f;
//     m_dropfile_event = false;

//     m_mousebutton_clicked[0] = false;
//     m_mousebutton_clicked[1] = false;
//     m_mousebutton_clicked[2] = false;

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
//                 // m_mousewheel_delta = e.wheel.y;
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
