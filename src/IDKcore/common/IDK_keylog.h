#pragma once

#include <SDL2/SDL.h>
#include "IDKdecl.h"


class idk::Keylog
{
private:
    bool _key_down[SDL_NUM_SCANCODES] = { false };
    bool _key_up[SDL_NUM_SCANCODES] = { false };
    bool _key_tapped[SDL_NUM_SCANCODES] = { false };

public:

    void log(const Uint8 *state)
    {
        for (int i=0; i<SDL_NUM_SCANCODES; i++)
        {
            _key_tapped[i] = false;

            if (_key_down[i] == false && state[i] == true)
                _key_tapped[i] = true;

            if (_key_down[i] == true && state[i] == false)
                _key_up[i] = true;

            else
                _key_up[i] = false;

            _key_down[i] = state[i];
        }
    };

    // void setPressed(SDL_Scancode scancode)
    // {
    //     _key_down[scancode] = true;
    // };

    bool keyDown(SDL_Scancode scancode) const
    {
        return _key_down[scancode];
    };

    bool keyUp(SDL_Scancode scancode) const
    {
        return _key_up[scancode];
    };

    bool keyTapped(SDL_Scancode scancode) const
    {
        return _key_tapped[scancode];
    };


};

