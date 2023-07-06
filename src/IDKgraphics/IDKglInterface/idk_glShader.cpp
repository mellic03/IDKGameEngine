#include "idk_glShader.h"

#include <string>



idk::PointlightUniformLocs::PointlightUniformLocs( GLuint program, int idx )
{
    std::string left = "un_pointlights[" + std::to_string(idx) + "].";

    loc_position  = gl::getUniformLocation(program, left + "position");
    loc_diffuse   = gl::getUniformLocation(program, left + "diffuse");
    loc_ambient   = gl::getUniformLocation(program, left + "ambient");

    loc_attenuation_constant   = gl::getUniformLocation(program, left + "attenuation_constant");
    loc_attentuation_linear    = gl::getUniformLocation(program, left + "attentuation_linear");
    loc_attentuation_quadratic = gl::getUniformLocation(program, left + "attentuation_quadratic");
}


idk::gbuffer_geometrypass_UniformLocs::gbuffer_geometrypass_UniformLocs( GLuint program )
{
    loc_viewpos           = gl::getUniformLocation(program, "un_viewpos");
   
    loc_num_pointlights   = gl::getUniformLocation(program, "un_num_pointlights");
    loc_num_spotlights    = gl::getUniformLocation(program, "un_num_spotlights");

    loc_albedo_texture    = gl::getUniformLocation(program, "un_albedo_texture");
    loc_specular_texture  = gl::getUniformLocation(program, "un_specular_texture");
    loc_normal_texture    = gl::getUniformLocation(program, "un_normal_texture");
    loc_specular_exponent = gl::getUniformLocation(program, "un_specular_exponent");

    for (int i=0; i<10; i++)
    {
        loc_pointlights.push_back(PointlightUniformLocs(program, i));
    }

}

