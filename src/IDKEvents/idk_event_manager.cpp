#include "idk_event_manager.hpp"

#include <filesystem>
namespace fs = std::filesystem;



idk::EventSystem::EventSystem()
{
    m_windowevents[0] = false;
    m_windowevents[1] = false;

    m_mousebutton_down = std::vector<bool>(3, false);
    m_mousebutton_up   = std::vector<bool>(3, false);
}


void
idk::EventSystem::onWindowEvent( WindowEvent winevent, std::function<void()> response )
{
    int idx = static_cast<int>(winevent);
    auto &winevents = m_windowevents;
    
    auto trigger = [&winevents, idx]()
    {
        return winevents[idx] == true;
    };

    m_events.create(  { trigger, response }  );
}


void
idk::EventSystem::onDropFile( const std::string &extension, std::function<void(const char *)> callback )
{
    m_dropfile_set[extension] = true;
    m_dropfile_callbacks[extension] = callback;
};



void
idk::EventSystem::onKeyEvent( idk::Keycode keycode, idk::KeyEvent keyevent, std::function<void()> callback )
{
    idk::Keylog &keylog = m_keylog;

    auto trigger = [keycode, keyevent, &keylog]()
    {
        return keylog.get(keycode, keyevent) == true;
    };

    m_events.create( {trigger, callback} );
}



void
idk::EventSystem::mouseCapture( bool capture )
{
    m_mouse_captured = capture;
    SDL_SetRelativeMouseMode( (capture) ? SDL_TRUE : SDL_FALSE );
}

void
idk::EventSystem::onMouseWheel( std::function<void(float f)> callback )
{
    m_scroll_events.push_back(callback);
}


void
idk::EventSystem::onKeyDown( idk::Keycode keycode, std::function<void()> callback )
{
    KeyEventWrapper wrapper;
    wrapper.keycode  = keycode;
    wrapper.callback = callback;

    m_KeyDown_events.push_back(wrapper);
}


void
idk::EventSystem::onKeyUp( idk::Keycode keycode, std::function<void()> callback )
{
    KeyEventWrapper wrapper;
    wrapper.keycode  = keycode;
    wrapper.callback = callback;

    m_KeyUp_events.push_back(wrapper);
}


void
idk::EventSystem::onKeyTapped( idk::Keycode keycode, std::function<void()> callback )
{
    KeyEventWrapper wrapper;
    wrapper.keycode  = keycode;
    wrapper.callback = callback;

    m_KeyTapped_events.push_back(wrapper);
}




#ifdef IDK_SDL2
void
idk::EventSystem::processMouseInput()
{
    // Reset window events
    for (bool &b: m_windowevents)
    {
        b = false;
    }

    m_mousewheel_delta = 0.0f;
    m_dropfile_event = false;


    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
                m_windowevents[(int)(WindowEvent::EXIT)] = true;
            break;


            case SDL_DROPFILE:
                m_dropfile_event = true;
                m_dropfile_path  = e.drop.file;
                {
                    std::string extension = fs::path(m_dropfile_path).extension();

                    if (m_dropfile_set[extension])
                    {
                        m_dropfile_callbacks[extension](e.drop.file);
                    }
                }
            break;


            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    m_windowevents[(int)(WindowEvent::EXIT)] = true;
                }
                else if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    m_windowevents[(int)(WindowEvent::RESIZE)] = true;
                    m_size.x = e.window.data1;
                    m_size.y = e.window.data2;
                }
            break;


            case (SDL_MOUSEBUTTONDOWN):
                m_mousebutton_down[e.button.button - 1] = true;
                m_mousebutton_up[e.button.button - 1] = false;
            break;


            case (SDL_MOUSEBUTTONUP):
                m_mousebutton_down[e.button.button - 1] = false;
                m_mousebutton_up[e.button.button - 1] = true;
            break;


            case (SDL_MOUSEWHEEL):
                m_mousewheel_delta = e.wheel.y;
            break;
        }

        if (m_SDL_pollevents.size() > 0)
        {
            m_SDL_pollevents.back()(&e);
        }
    }

    int x, y;

    SDL_GetMouseState(&x, &y);
    m_mouse_position.x = float(x);
    m_mouse_position.y = float(y);

    SDL_GetRelativeMouseState(&x, &y);
    m_mouse_delta.x = float(x);
    m_mouse_delta.y = float(y);
}

#elif defined(IDK_SFML)
void
idk::EventSystem::processMouseInput()
{

}
#endif



#ifdef IDK_SDL2
void
idk::EventSystem::processKeyInput()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    m_keylog.log(state);
}

#elif defined(IDK_SFML)
void
idk::EventSystem::processKeyInput()
{

}
#endif


void
idk::EventSystem::update()
{
    for (idk::Event &event: m_events)
    {
        if (event.trigger())
        {
            event.response();
        }
    }

    for (KeyEventWrapper wrapper: m_KeyDown_events)
        if (m_keylog.keyDown(wrapper.keycode))
            wrapper.callback();

    for (KeyEventWrapper wrapper: m_KeyUp_events)
        if (m_keylog.keyUp(wrapper.keycode))
            wrapper.callback();

    for (KeyEventWrapper wrapper: m_KeyTapped_events)
        if (m_keylog.keyTapped(wrapper.keycode))
            wrapper.callback();

    if (m_mousewheel_delta != 0.0f && mouseCaptured())
    {
        for (mousefun_t callback: m_scroll_events)
        {
            callback(m_mousewheel_delta);
        }
    }

};

