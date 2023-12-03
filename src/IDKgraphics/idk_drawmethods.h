#pragma once

#include "model/IDKmodel.hpp"
#include "libidk/IDKgl.hpp"

namespace idk::drawmethods
{
    void    bind_material( glShader &, Material & );
    void    draw_textured( glShader &, Model &, glm::mat4 &, Allocator<Material> & );
    void    draw_untextured( glShader &, Model &, glm::mat4 & );
    void    draw_wireframe( glShader &, Model &, glm::mat4 & );

};

