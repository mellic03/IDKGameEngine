#include "idk_keylog.hpp"



idk::Keylog::Keylog(): m_keys(3)
{
    m_keys.resize(3);

    for (int i=0; i<3; i++)
        m_keys[i] = std::vector<bool>(idk::Keycode::NUM_KEYCODES, false);
}


void
idk::Keylog::log(const Uint8 *state)
{
    for (int i=0; i<idk::Keycode::NUM_KEYCODES; i++)
    {
        m_keys[static_cast<int>(idk::KeyEvent::TAPPED)][i] = false;
        bool down = m_keys[static_cast<int>(idk::KeyEvent::DOWN)][i];

        if (down == false && state[i] == true)
        {
            m_keys[static_cast<int>(idk::KeyEvent::TAPPED)][i] = true;
        }

        if (down == true && state[i] == false)
        {
            m_keys[static_cast<int>(idk::KeyEvent::UP)][i] = true;
        }

        else
        {
            m_keys[static_cast<int>(idk::KeyEvent::UP)][i] = false;
        }

        m_keys[static_cast<int>(idk::KeyEvent::DOWN)][i] = state[i];
    }
}


bool
idk::Keylog::get( idk::Keycode keycode, idk::KeyEvent event )
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
idk::Keylog::keyTapped(idk::Keycode keycode)
{
    return m_keys[static_cast<int>(KeyEvent::TAPPED)][keycode];
}


