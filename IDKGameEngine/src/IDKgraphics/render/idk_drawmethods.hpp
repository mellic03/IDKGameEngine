#pragma once

#include <libidk/GL/common.hpp>
#include <libidk/idk_export.hpp>

#include "../model/IDKmodel.hpp"
#include "../idk_model_manager.hpp"
#include "../animation/IDKanimation.hpp"



namespace idk::drawmethods
{

    void    bind_material( const std::string &name, glShader &, Material & );
    void    bind_material( glShader &, Material & );


    void    dummy( glShader &, int, const glm::mat4 &, ModelSystem & );


    void    IDK_VISIBLE draw_textured     ( glShader &, int model_id, const glm::mat4 &, ModelSystem & );
    void    IDK_VISIBLE draw_untextured   ( glShader &, int model_id, const glm::mat4 &, ModelSystem & );
    void    IDK_VISIBLE draw_heightmapped ( glShader &, int model_id, const glm::mat4 &, ModelSystem & );
    void    IDK_VISIBLE draw_instanced    ( glShader &, int model_id, const glm::mat4 &, ModelSystem & );
    void    IDK_VISIBLE draw_indirect     ( glShader &, int model_id, const glm::mat4 &, ModelSystem & );


    void    IDK_VISIBLE draw_animated( float dtime, glUBO &, int animator, int model,
                           glShader &, glm::mat4 &, ModelSystem & );


    void    draw_wireframe( glShader &, Model &, glm::mat4 & );

};

