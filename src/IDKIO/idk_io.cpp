#include "idk_io.hpp"
#include <libidk/idk_log.hpp>
#include <libidk/idk_assert.hpp>
#include <filesystem>
using namespace idk;
using enum idk::CallbackMask;
using lf = idk::log_flag;




idk::IO::IO()
{

}





bool
IO::windowEvent( uint32_t event )
{
    return m_windowevents[event];
}


static void
findDevice( idk::DeviceController *ctl, std::unordered_map<int, idk::Device*> &devices )
{
    for (auto &[dev_key, dev]: devices)
    {
        // if (dev->ctl)
        // {
        //     LOG_ADV(lf::IO|lf::DETAIL, "Device {} already being controlled", dev_key);
        //     continue;
        // }

        if (ctl->canListen(dev))
        {
            dev->addListener(ctl);
            LOG_ADV(
                lf::IO|lf::DEBUG, "Ctl {} now listening to Dev {}", ctl->ID, dev_key
            );
            return;
        }

        else
        {
            // LOG_ADV(lf::IO|lf::DETAIL, "Ctl {} cannot control Dev {} ", ctl->ID, dev_key);
        }
    }

    // LOG_ADV(lf::IO|lf::WARN, "Failed to find port for device {}", ctl->ID);
}



void
idk::IO::update( uint64_t msElapsed )
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    m_keylog.update(state, msElapsed);

    for (auto &[id, ctl]: m_controllers)
    {
        IDK_ASSERT("Something is very wrong here", ctl != nullptr);

        if (ctl->isListening())
        {
            ctl->update(uint32_t(msElapsed));
        }

        else
        {
            findDevice(ctl, m_devices);
        }
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

        _processEvents();
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
maskToIdx( idk::CallbackMask mask )
{
    int idx;

    switch (mask)
    {
        using enum idk::CallbackMask;

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
        auto msg = std::format("Invalid CallbackMask {}", uint32_t(mask));
        IDK_ASSERT(msg.c_str(), false);
    }

    return idx;
}


int IO::onPollEvent( std::function<void(SDL_Event*)> callback )
{
    return _createCallback(POLL_EVENTS, m_callbacks.pollEvents.create(callback));
}

int IO::onMouseClick( CallbackMask mask, const std::function<void()> &fn )
{
    int idx = maskToIdx(mask);
    return _createCallback(mask, m_callbacks.mouseclick[idx].create(fn));
}

int IO::onMouseMotion( const std::function<void(float, float)> &fn )
{
    return _createCallback(MOUSE_MOTION, m_callbacks.mousemotion.create(fn));
}

int IO::onMouseWheel( const std::function<void(float, float)> &fn )
{
    return _createCallback(MOUSE_WHEEL, m_callbacks.mousewheel.create(fn));
}

int IO::onKey( const std::function<void(idk::Keycode)> &fn )
{
    return _createCallback(KEY_PRESS, m_callbacks.keypress.create(fn));
}

int IO::onFileDrop( const std::string &ext, const std::function<void(const std::string&)> &fn )
{
    return _createCallback(FILE_DROP, m_callbacks.dropfile[ext].create(fn));
}


int IO::_createCallback( uint32_t type, int32_t idx )
{
    LOG_ADV(lf::INFO, "Created callback of type {} with id {}", type, idx);
    return m_callbacks.handles.create({type, idx});
}

void IO::removeCallback( int id )
{
    auto &handle = m_callbacks.handles.get(id);

    switch (handle.mask)
    {
        case POLL_EVENTS:  m_callbacks.pollEvents.destroy(handle.idx);    break;
        case MOUSE_LEFT:   m_callbacks.mouseclick[0].destroy(handle.idx); break;
        case MOUSE_MID:    m_callbacks.mouseclick[1].destroy(handle.idx); break;
        case MOUSE_RIGHT:  m_callbacks.mouseclick[2].destroy(handle.idx); break;
        case MOUSE_MOTION: m_callbacks.mousemotion.destroy(handle.idx);   break;
        case MOUSE_WHEEL:  m_callbacks.mousemotion.destroy(handle.idx);   break;
        case KEY_PRESS:    m_callbacks.keypress.destroy(handle.idx);      break;

        default: LOG_ADV(lf::ERROR, "Unknown callback type {}", handle.mask); return;
    };

    m_callbacks.handles.destroy(id);

    LOG_ADV(lf::INFO, "Removed callback of type {} with id {}", handle.mask, handle.idx);
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

bool      IO::keyDown     ( idk::Keycode key ) { return m_keylog.keyDown(key);     }
bool      IO::keyUp       ( idk::Keycode key ) { return m_keylog.keyUp(key);       }
bool      IO::keyReleased ( idk::Keycode key ) { return m_keylog.keyReleased(key); }
bool      IO::keyPressed  ( idk::Keycode key ) { return m_keylog.keyPressed(key);  }
bool      IO::keyTapped   ( idk::Keycode key ) { return m_keylog.keyTapped(key);   }





void
idk::IO::createDevice( int which )
{
    IDK_ASSERT("Device already exists", m_devices.contains(which) == false);

    auto *impl = SDL_GameControllerOpen(which);
    if (impl == nullptr)
    {
        LOG_ADV(lf::IO|lf::ERROR, "Attempted to open controller on invalid index {}", which);
        return;
    }

    which = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(impl));

    m_devices[which] = new idk::Device(*this);
    m_devices[which]->impl  = impl;
    m_devices[which]->which = which;

    LOG_ADV(lf::IO|lf::INFO, "Created new Device {}", which);
}

std::unordered_map<int, idk::Device*>&
idk::IO::getDevices()
{
    return m_devices;
}

idk::Device*
idk::IO::getDevice( int which )
{
    IDK_ASSERT("No such device", m_devices.contains(which));
    return m_devices[which];
}

void
idk::IO::destroyDevice( int which )
{
    IDK_ASSERT("No such device", m_devices.contains(which));
    delete m_devices[which];
    m_devices.erase(which);
}



idk::DeviceController*
idk::IO::getController( int id )
{
    return m_controllers.get(id);
}

