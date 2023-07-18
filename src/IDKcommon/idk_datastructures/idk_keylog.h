#pragma once

#include <SDL2/SDL.h>

#ifdef SDL_h_
    #include "../IDK_SDL2/idk_SDL_keybindings.h"

#elif defined(SFML_h_)
    #include "../idk_SFML_keybindings.h"

#endif


namespace idk { class Keylog; };


class idk::Keylog
{
private:
    bool m_key_down[idk_keycode::NUM_KEYCODES] = { false };
    bool m_key_up[idk_keycode::NUM_KEYCODES] = { false };
    bool m_key_tapped[idk_keycode::NUM_KEYCODES] = { false };

public:

    void log(const Uint8 *state)
    {
        for (int i=0; i<idk_keycode::NUM_KEYCODES; i++)
        {
            m_key_tapped[i] = false;

            if (m_key_down[i] == false && state[i] == true)
                m_key_tapped[i] = true;

            if (m_key_down[i] == true && state[i] == false)
                m_key_up[i] = true;

            else
                m_key_up[i] = false;

            m_key_down[i] = state[i];
        }
    };

    bool keyDown(idk_keycode keycode) const
    {
        return m_key_down[keycode];
    };

    bool keyUp(idk_keycode keycode) const
    {
        return m_key_up[keycode];
    };

    bool keyTapped(idk_keycode keycode) const
    {
        return m_key_tapped[keycode];
    };
};
