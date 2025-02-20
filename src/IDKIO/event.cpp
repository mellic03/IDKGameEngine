#include "idk_io.hpp"
#include <libidk/idk_log2.hpp>
#include <libidk/idk_assert.hpp>
#include <filesystem>
using namespace idk;


void
IO::processEvent( SDL_Event &e )
{
    namespace fs = std::filesystem;

    switch (e.type)
    {
        default: break;

        case SDL_QUIT:
            m_windowevents[IO::WIN_EXIT] = true;
            break;

        case SDL_DROPFILE:
        {
            std::string path = e.drop.file;
            std::string ext  = fs::path(path).extension().string();

            for (auto &[id, callback]: m_callbacks.dropfile[ext])
            {
                callback(path);
            }

            break;
        }

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
            int idx = e.button.button - 1;

            for (auto &[id, callback]: m_callbacks.mouseclick[idx])
            {
                callback();
            }

            m_mouse.down[idx] = true;
            m_mouse.up[idx] = false;
            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            int idx = e.button.button - 1;

            for (auto &[id, callback]: m_callbacks.mouseclick[idx])
            {
                callback();
            }

            bool prev_down = m_mouse.down[idx];
            bool prev_up   = m_mouse.up[idx];

            m_mouse.down[idx] = false;
            m_mouse.up[idx] = true;

            bool curr_down = m_mouse.down[idx];
            bool curr_up   = m_mouse.up[idx];

            m_mouse.clicked[idx] = (prev_down == true && curr_down == false);
            break;
        }

        case SDL_MOUSEMOTION:
        {
            for (auto &[id, callback]: m_callbacks.mousemotion)
                callback(e.motion.xrel, e.motion.yrel);
            break;
        }

        case SDL_MOUSEWHEEL:
        {
            for (auto &[id, callback]: m_callbacks.mousewheel)
            {
                callback(e.wheel.x, e.wheel.y);
            }

            m_mouse.wheel_delta = e.wheel.y;
            break;
        }

        case SDL_KEYDOWN:
        {
            auto key = static_cast<idk::Keycode>(e.key.keysym.scancode);
            for (auto &[id, callback]: m_callbacks.keypress)
                callback(key);
            break;
        }

        case SDL_JOYAXISMOTION:
            m_joystick_jaxis[e.jaxis.axis] = e.jaxis.value;
            for (auto &[id, callback]: m_callbacks.joystickaxis)
            {
                callback(uint8_t(e.jaxis.axis), e.jaxis.value);
            }
            break;

        case SDL_JOYHATMOTION:
            m_joystick_jhat[e.jhat.hat] = e.jhat.value; 
            break;

        case SDL_JOYBUTTONDOWN:
            m_joystick_btndown[e.jbutton.button] = true;
            break;

        case SDL_JOYBUTTONUP:
            m_joystick_btndown[e.jbutton.button] = false;
            break;
    }

    processEventGamepad(e);

    m_poll_callback(&e);
}



void
IO::processEventGamepad( SDL_Event &e )
{
    switch (e.type)
    {
        default: break;

        case SDL_CONTROLLERBUTTONDOWN:
        {
            m_gpad.setBtn(e.cbutton.button, true);
            for (auto &[id, callback]: m_callbacks.gpadbtn[IDX_GPAD_DOWN])
                callback(e.cbutton.button);
            break;
        }

        case SDL_CONTROLLERBUTTONUP:
        {
            m_gpad.setBtn(e.cbutton.button, false);
            for (auto &[id, callback]: m_callbacks.gpadbtn[IDX_GPAD_UP])
                callback(e.cbutton.button);
            break;
        }

        case SDL_CONTROLLERAXISMOTION:
        {
            m_gpad.setAxis(e.caxis.axis, e.caxis.value);
            break;
        }

        case SDL_CONTROLLERDEVICEADDED:
        {
            m_gpad.open(e.cdevice.which);

            LOG_INFO(
                "idk::IO::processEvent",
                std::format("Controller detected on device index {}", e.cdevice.which)
            );

            break;
        }

        case SDL_CONTROLLERDEVICEREMOVED:
        {
            if (e.cdevice.which == m_gpad.id)
            {
                m_gpad.close();
            
                for (int i=0; i<SDL_NumJoysticks(); i++)
                {
                    if (SDL_IsGameController(i))
                    {
                        m_gpad.open(i);
                        break;
                    }
                }
            }
                
            break;
        }
    }

}
