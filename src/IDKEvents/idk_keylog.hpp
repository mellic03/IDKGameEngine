#pragma once

#include <libidk/idk_sdl2.hpp>

#include <vector>


namespace idk
{
    namespace KeyEvent
    {
        enum key_event { UP=0, DOWN=1, PRESSED=2, RELEASED=3, TAPPED=4 };
    }

    class Keylog;
};


class idk::Keylog
{
private:
    std::vector<std::vector<bool>>   m_keys;
    std::vector<uint64_t>            m_timers;
    uint64_t msElapsed = 10;

public:

            Keylog();

    void    update      ( const Uint8 *state, uint64_t ms );
    void    log         ( SDL_Keycode key, bool down );
    bool    get         ( idk::Keycode keycode, idk::KeyEvent::key_event event );
    bool    keyDown     ( idk::Keycode keycode );
    bool    keyUp       ( idk::Keycode keycode );
    bool    keyPressed  ( idk::Keycode keycode );
    bool    keyReleased ( idk::Keycode keycode );
    bool    keyTapped   ( idk::Keycode keycode );


    bool keysTapped( idk::Keycode first )
    {
        return keyTapped(first);
    };

    template <typename... Args>
    bool keysTapped( idk::Keycode first, Args ...rest )
    {
        return keyTapped(first) && keysTapped(rest...);
    };
};
