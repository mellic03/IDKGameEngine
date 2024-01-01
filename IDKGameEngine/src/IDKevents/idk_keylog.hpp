#pragma once

#include <libidk/IDKSDL2.hpp>
#include <libidk/idk_SDL_keybindings.hpp>

#include <libidk/idk_export.hpp>

#include <vector>


namespace idk
{
    enum class KeyEvent { UP, DOWN, TAPPED };
    class Keylog;
};


class IDK_VISIBLE idk::Keylog
{
private:
    std::vector<std::vector<bool>>   m_keys;

public:

            Keylog();

    void    log       ( const Uint8 *state );
    bool    get       ( idk::Keycode keycode, idk::KeyEvent event );
    bool    keyDown   ( idk::Keycode keycode );
    bool    keyUp     ( idk::Keycode keycode );
    bool    keyTapped ( idk::Keycode keycode );
};
