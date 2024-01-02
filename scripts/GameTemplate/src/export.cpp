#include "GameTemplate.hpp"
#include <libidk/idk_export.hpp>


IDK_EXPORT_API
(
    idk::Game *getInstance()
    {
        return reinterpret_cast<idk::Game *>(new GameTemplate("Game Template"));
    }
)

