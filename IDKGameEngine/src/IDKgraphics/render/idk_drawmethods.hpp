#pragma once

#include <libidk/IDKgl.hpp>
#include "../model/IDKmodel.hpp"
#include "../idk_model_manager.hpp"
#include "../animation/IDKanimation.hpp"


namespace idk::drawmethods
{
    void    bind_material( glShader &, Material & );
    void    draw_textured( glShader &, Model &, glm::mat4 &, Allocator<Material> & );

    void    draw_animated( float dtime, glUBO &, int animator, int model,
                           glShader &, glm::mat4 &, ModelSystem & );

    void    draw_untextured( glShader &, Model &, glm::mat4 & );
    void    draw_wireframe( glShader &, Model &, glm::mat4 & );

};

