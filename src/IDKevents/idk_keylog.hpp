#pragma once

#include "libidk/libidk.h"

// #ifdef SDL_h_
//     #include "../IDK_SDL2/idk_SDL_keybindings.h"

// #elif defined(SFML_h_)
//     #include "../idk_SFML_keybindings.h"

// #endif


namespace idk
{
    enum class KeyEvent { UP, DOWN, TAPPED };
    class Keylog;
};


class idk::Keylog
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
