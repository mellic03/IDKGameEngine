#include "idk_event_manager.h"



idk::EventManager::EventManager()
{
    m_windowevents = idk::vector<bool>(
        static_cast<int>(WindowEvent::NUM_EVENTS),
        false
    );

    m_mousebutton_down = idk::vector<bool>(3, false);
    m_mousebutton_up   = idk::vector<bool>(3, false);
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
idk::EventManager::mouseCapture( bool capture )
{
    m_mouse_captured = capture;
    SDL_SetRelativeMouseMode( (capture) ? SDL_TRUE : SDL_FALSE );
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

    while (SDL_PollEvent(&m_SDL_Event))
    {
        switch (m_SDL_Event.type)
        {
            case (SDL_QUIT):
                m_windowevents[(int)(WindowEvent::EXIT)] = true;
            break;


            case SDL_WINDOWEVENT:
                if (m_SDL_Event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    m_windowevents[(int)(WindowEvent::EXIT)] = true;
                }
                else if (m_SDL_Event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    m_windowevents[(int)(WindowEvent::RESIZE)] = true;
                    m_window_data.width  = m_SDL_Event.window.data1;
                    m_window_data.height = m_SDL_Event.window.data2;
                }
            break;


            case (SDL_MOUSEBUTTONDOWN):
                m_mousebutton_down[m_SDL_Event.button.button - 1] = true;
                m_mousebutton_up[m_SDL_Event.button.button - 1] = false;
            break;


            case (SDL_MOUSEBUTTONUP):
                m_mousebutton_down[m_SDL_Event.button.button - 1] = false;
                m_mousebutton_up[m_SDL_Event.button.button - 1] = true;
            break;

        }
    
        for (auto &fn: m_SDL_pollevents)
        {
            fn(&m_SDL_Event);
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
    m_events.for_each([](idk::Event &event)
    {
        if (event.trigger())
        {
            event.response();
        }
    });

};

