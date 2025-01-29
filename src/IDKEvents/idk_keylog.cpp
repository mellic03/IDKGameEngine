#include "idk_keylog.hpp"
#include <iostream>


idk::Keylog::Keylog()
:   m_keys(5),
    m_timers(idk::Keycode::NUM_KEYCODES, 0)
{
    m_keys.resize(5);

    for (int i=0; i<5; i++)
    {
        m_keys[i] = std::vector<bool>(idk::Keycode::NUM_KEYCODES, false);
    }
}


void
idk::Keylog::update( const Uint8 *state, uint64_t ms )
{
    for (uint32_t i=0; i<idk::Keycode::NUM_KEYCODES; i++)
    {
        m_keys[KeyEvent::TAPPED][i] = false;

        bool was_down = m_keys[KeyEvent::DOWN][i];
        bool is_down  = state[i];

        m_keys[KeyEvent::PRESSED][i] = !was_down && is_down;

        if (is_down)
        {
            if (was_down) m_timers[i] += ms;
            else          m_timers[i]  = 0;

            m_keys[KeyEvent::RELEASED][i] = false;
        }

        if (is_down == false && was_down == true && m_timers[i] < 250)
        {
            m_keys[KeyEvent::TAPPED][i] = true;
        }

        if (was_down == true && is_down == false)
        {
            m_keys[KeyEvent::RELEASED][i] = true;
        }

        if (was_down == true && is_down == false)
        {
            m_keys[KeyEvent::UP][i] = true;
        }

        else
        {
            m_keys[KeyEvent::UP][i] = false;
        }

        m_keys[KeyEvent::DOWN][i] = is_down;
    }
}


void
idk::Keylog::log( SDL_Keycode key, bool is_down )
{
    m_keys[KeyEvent::TAPPED][key] = false;
    bool was_down = m_keys[KeyEvent::DOWN][key];

    if (was_down == false && is_down == true)
    {
        m_keys[KeyEvent::TAPPED][key] = true;
    }

    if (was_down == true && is_down == false)
    {
        m_keys[KeyEvent::UP][key] = true;
    }

    else
    {
        m_keys[KeyEvent::UP][key] = false;
    }

    m_keys[KeyEvent::DOWN][key] = is_down;
}


bool
idk::Keylog::get( idk::Keycode keycode, idk::KeyEvent::key_event event )
{
    return m_keys[static_cast<int>(event)][keycode];
}


bool
idk::Keylog::keyDown(idk::Keycode keycode)
{
    return m_keys[static_cast<int>(KeyEvent::DOWN)][keycode];
}


bool
idk::Keylog::keyUp(idk::Keycode keycode)
{
    return m_keys[static_cast<int>(KeyEvent::UP)][keycode];
}


bool
idk::Keylog::keyPressed(idk::Keycode keycode)
{
    return m_keys[static_cast<int>(KeyEvent::PRESSED)][keycode];
}


bool
idk::Keylog::keyReleased(idk::Keycode keycode)
{
    return m_keys[static_cast<int>(KeyEvent::RELEASED)][keycode];
}


bool
idk::Keylog::keyTapped(idk::Keycode keycode)
{
    return m_keys[static_cast<int>(KeyEvent::TAPPED)][keycode];
}


