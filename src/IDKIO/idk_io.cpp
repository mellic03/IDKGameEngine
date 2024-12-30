#include "idk_io.hpp"

using namespace idk;
idk::IO *idk::idkio = new idk::IO();


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
    if (e.type == SDL_QUIT)
    {
        // m_win_callbacks[IO::WIN_EXIT]();
        m_windowevents[IO::WIN_EXIT] = true;
    }

    else if (e.type == SDL_DROPFILE)
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
    }

    else if (e.type == SDL_WINDOWEVENT)
    {
        if (e.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            m_windowevents[IO::WIN_EXIT] = true;
        }

        else if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            m_windowevents[IO::WIN_RESIZE] = true;
        }
    }

    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        // Good for callbacks?
        // x = e.button.x;
        // y = e.button.y;

        m_mousebutton_down[e.button.button - 1] = true;
        m_mousebutton_up[e.button.button - 1] = false;
    }

    else if (e.type == SDL_MOUSEBUTTONUP)
    {
        bool prev_down = m_mousebutton_down[e.button.button - 1];
        bool prev_up   = m_mousebutton_up[e.button.button - 1];

        m_mousebutton_down[e.button.button - 1] = false;
        m_mousebutton_up[e.button.button - 1] = true;

        bool curr_down = m_mousebutton_down[e.button.button - 1];
        bool curr_up   = m_mousebutton_up[e.button.button - 1];

        m_mousebutton_clicked[e.button.button-1] = (prev_down == true && curr_down == false);
    }

    else if (e.type == SDL_MOUSEWHEEL)
    {
        m_mousewheel_delta = e.wheel.y;
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



void
idk::IO::update( float dt )
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    m_keylog.log(state);

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




void      IO::setViewportOffset( int x, int y ) { m_Moff = glm::vec2(x, y); }

void      IO::mouseCapture( bool b ) { SDL_SetRelativeMouseMode((b) ? SDL_FALSE: SDL_TRUE); }
bool      IO::mouseCaptured()        { return m_Mcaptured;     }
glm::vec2 IO::mousePosition()        { return m_Mpos; }
glm::vec2 IO::mouseDelta()           { return m_Mdelta;        }

bool      IO::mouseUp         ( uint32_t b )  { return m_mousebutton_up[b];      }
bool      IO::mouseDown       ( uint32_t b )  { return m_mousebutton_down[b];    }
bool      IO::mouseClicked    ( uint32_t b )  { return m_mousebutton_clicked[b]; }
float     IO::mouseWheelDelta (            )  { return m_mousewheel_delta;       }



bool      IO::keyDown   ( idk::Keycode key ) { return m_keylog.keyDown(key);   }
bool      IO::keyUp     ( idk::Keycode key ) { return m_keylog.keyUp(key);     }
bool      IO::keyTapped ( idk::Keycode key ) { return m_keylog.keyTapped(key); }





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
