#pragma once

#include "IDKmodel/IDKmodel.h"
#include "IDKgl/IDKgl.hpp"

namespace idk::drawmethods
{
    void    bind_material( glShader &, Material & );
    void    draw_textured( glShader &, Model &, Transform &, Allocator<Material> & );
    void    draw_untextured( glShader &, Model &, Transform & );
    void    draw_wireframe( glShader &, Model &, Transform & );

};

