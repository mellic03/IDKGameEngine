#pragma once

#include "IDKmodel/IDKmodel.h"
#include "IDKglInterface/IDKglInterface.hpp"

namespace idk::drawmethods
{
    void    bind_material( Material & );
    void    draw_textured( Model &, Transform &, Allocator<Material> & );
    void    draw_untextured( Model &, Transform & );
    void    draw_wireframe( Model &, Transform & );

};

