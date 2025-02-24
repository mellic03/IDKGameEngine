#include "idk_io.hpp"
#include <libidk/idk_log.hpp>
#include <libidk/idk_assert.hpp>
#include <filesystem>

using namespace idk;
using lf = idk::log_flag;


static constexpr bool IsControllerEvent( uint32_t type )
{
    return (SDL_CONTROLLERAXISMOTION <= type && type <= SDL_CONTROLLERSTEAMHANDLEUPDATED);
}


void
IO::_processEvents()
{
    static SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        for (auto &[id, callback]: m_callbacks.pollEvents)
        {
            callback(&e);
        }

        _processEvent(e);
        _processEventController(e);
    }
}


void
IO::_processEvent( SDL_Event &e )
{
    namespace fs = std::filesystem;

    switch (e.type)
    {
        default: return;

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

        // case SDL_JOYAXISMOTION: m_jstick.setAxis(e.jaxis.axis, e.jaxis.value); break;
        // case SDL_JOYHATMOTION:  m_jstick.setHat(e.jhat.hat, e.jhat.value);     break;
        // case SDL_JOYBUTTONDOWN: m_joystick_btndown[e.jbutton.button] = true;   break;
        // case SDL_JOYBUTTONUP:   m_joystick_btndown[e.jbutton.button] = false;  break;
    }
}



void
IO::_processEventController( SDL_Event &e )
{
    int which = -1;

    if (e.type == SDL_CONTROLLERDEVICEADDED)
    {
        which = e.cdevice.which;

        LOG_ADV(lf::IO|lf::DETAIL, "SDL_CONTROLLERDEVICEADDED, which={}", which);
        auto *impl = SDL_GameControllerOpen(which);
        // int instanceID = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(impl));

        if (m_devices.contains(which) == false)
        {
            createDevice(which);
        }

        // if (getDevice(which)->isOpen() == false)
        // {
        //     getDevice(which)->open(which);
        // }
    }

    // SDL_GameControllerGetPlayerIndex
    // SDL_GameControllerOpen
    // SDL_GameControllerGetPlayerIndex

    else if (e.type == SDL_CONTROLLERDEVICEREMOVED)
    {
        LOG_ADV(lf::IO|lf::DETAIL, "SDL_CONTROLLERDEVICEREMOVED, which={}", e.cdevice.which);
        destroyDevice(e.cdevice.which);
    }


    // which = e.cbutton.which;

    // if (m_devices.contains(which) == false)
    // {
    //     if (createDevice(which) == nullptr)
    //     {
    //         return;
    //     }
    // }

    switch (e.type)
    {
        default: break;

        case SDL_CONTROLLERAXISMOTION:
            // LOG_ADV(lf::IO|lf::DETAIL, "SDL_CONTROLLERAXISMOTION");
            getDevice(e.cbutton.which)->updateAxis(e.caxis.axis, e.caxis.value);
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            // LOG_ADV(lf::IO|lf::DETAIL, "SDL_CONTROLLERBUTTONDOWN");
            getDevice(e.cbutton.which)->updateButton(e.cbutton.button, true);
            break;
        case SDL_CONTROLLERBUTTONUP:
            // LOG_ADV(lf::IO|lf::DETAIL, "SDL_CONTROLLERBUTTONUP");
            getDevice(e.cbutton.which)->updateButton(e.cbutton.button, false);
            break;
    }

}
