#pragma once

#include "IDKdecl.h"


class idk::Keylog
{
public:

    bool keys_pressed[SDL_NUM_SCANCODES] = { false };

    void log(const Uint8 *state)
    {
        for (int i=0; i<SDL_NUM_SCANCODES; i++)
        this->keys_pressed[i] = state[i];
    }

    void clear(void)
    {
        for (auto &key_pressed: this->keys_pressed)
        key_pressed = false;
    };

    void setPressed(SDL_Scancode scancode)
    {
        this->keys_pressed[scancode] = true;
    };

    bool isPressed(SDL_Scancode scancode)
    {
        return this->keys_pressed[scancode];
    };

};

