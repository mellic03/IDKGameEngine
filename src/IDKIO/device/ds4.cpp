#include "ds4.hpp"
#include "gamepad.hpp"

#include <SDL2/SDL.h>
#include <libidk/idk_assert.hpp>
#include <libidk/idk_log.hpp>
#include <cstring>

using lf = idk::log_flag;

idk::DS4::DS4()
:   Gamepad()
{
    LOG_ADV(lf::IO|lf::DETAIL, "");
}


bool
idk::DS4::canBind( idk::Device *dev )
{
    return (SDL_GameControllerGetType(dev->impl) == SDL_CONTROLLER_TYPE_PS4);
}
