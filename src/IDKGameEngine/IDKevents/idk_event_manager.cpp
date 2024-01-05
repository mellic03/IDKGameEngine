#include "idk_event_manager.hpp"



idk::EventManager::EventManager()
{
    m_windowevents[0] = false;
    m_windowevents[1] = false;

    m_mousebutton_down = std::vector<bool>(3, false);
    m_mousebutton_up   = std::vector<bool>(3, false);
}


void
idk::EventManager::onWindowEvent( WindowEvent winevent, std::function<void()> response )
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
idk::EventManager::onDropFile( std::function<void(const char *)> callback )
{
    _dropfile_callback = callback;
};



void
idk::EventManager::onKeyEvent( idk::Keycode keycode, idk::KeyEvent keyevent, std::function<void()> callback )
{
    idk::Keylog &keylog = m_keylog;

    auto trigger = [keycode, keyevent, &keylog]()
    {
        return keylog.get(keycode, keyevent) == true;
    };

    m_events.create( {trigger, callback} );
}



void
idk::EventManager::mouseCapture( bool capture )
{
    m_mouse_captured = capture;
    SDL_SetRelativeMouseMode( (capture) ? SDL_TRUE : SDL_FALSE );
}

void
idk::EventManager::onMouseWheel( std::function<void(float f)> callback )
{
    m_scroll_events.push_back(callback);
}




#ifdef IDK_SDL2
void
idk::EventManager::processMouseInput()
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
                _dropfile_callback(e.drop.file);
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
    
        for (auto &fn: m_SDL_pollevents)
        {
            fn(&e);
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
idk::EventManager::processMouseInput()
{

}
#endif



#ifdef IDK_SDL2
void
idk::EventManager::processKeyInput()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    m_keylog.log(state);
}

#elif defined(IDK_SFML)
void
idk::EventManager::processKeyInput()
{

}
#endif


void
idk::EventManager::update()
{
    for (idk::Event &event: m_events)
    {
        if (event.trigger())
        {
            event.response();
        }
    }

    if (m_mousewheel_delta != 0.0f && mouseCaptured())
    {
        for (mousefun_t callback: m_scroll_events)
        {
            callback(m_mousewheel_delta);
        }
    }

};

