#include "idk_io.hpp"



void
idk::IO::update()
{
    
}



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
