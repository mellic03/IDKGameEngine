#pragma once

#include "idk_glBindings.h"


namespace idk
{


struct glVertexShader
{

};


struct glFragmentShader
{

};


struct PointlightUniformLocs
{
            PointlightUniformLocs( GLuint program, int idx );

    GLint   loc_position;
    GLint   loc_diffuse;
    GLint   loc_ambient;

    GLint   loc_attenuation_constant;
    GLint   loc_attentuation_linear;
    GLint   loc_attentuation_quadratic;
};


struct gbuffer_geometrypass_UniformLocs
{
                gbuffer_geometrypass_UniformLocs() {  };
                gbuffer_geometrypass_UniformLocs( GLuint program );

    // Uniform locations --------------------
    GLint       loc_viewpos;
    GLint       loc_num_pointlights;
    GLint       loc_num_spotlights;

    GLint       loc_albedo_texture;
    GLint       loc_specular_texture;
    GLint       loc_normal_texture;
    GLint       loc_specular_exponent;

    std::vector<PointlightUniformLocs>  loc_pointlights;
    // --------------------------------------
};





};