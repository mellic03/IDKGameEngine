#pragma once

#include "IDKmodel/IDKmodel.h"
#include "IDKglInterface/IDKglInterface.h"

namespace idk::drawmethods
{
    void    bind_material( Material & );
    void    draw_textured( Model &, Transform &, Allocator<Material> & );
    void    draw_untextured( Model &, Transform & );

};

