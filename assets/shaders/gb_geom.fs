#version 440 core

#include "structs.glsl"

#include "util.glsl"


layout (location = 0) out vec4 fsout_frag_color_0;
layout (location = 1) out vec4 fsout_frag_color_1;
layout (location = 2) out vec4 fsout_frag_color_2;
layout (location = 3) out vec4 fsout_frag_color_3;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec2 fsin_texcoords;

uniform vec3 un_viewpos;

uniform pointlight un_pointlights[10];
uniform int un_num_pointlights;

uniform spotlight un_spotlights[10];
uniform int un_num_spotlights;

uniform sampler2D un_albedo_texture;
uniform sampler2D un_specular_texture;
uniform sampler2D un_normal_texture;
uniform float un_specular_exponent;

#include "lighting/lightingmethods.glsl"


void main()
{
    vec3 view_dir = normalize(un_viewpos - fsin_fragpos);
    vec3 albedo_map = texture(un_albedo_texture, fsin_texcoords).rgb;
    vec3 specular_map = texture(un_specular_texture, fsin_texcoords).rgb;

    vec3 color = vec3(0.0);
    for (int i = 0; i < un_num_pointlights; i++)
    {
        color += pointlight_contribution(i, view_dir, albedo_map, specular_map, un_specular_exponent);
    }

    for (int i = 0; i < un_num_spotlights; i++)
    {
        color += spotlight_contribution(i, view_dir, albedo_map, specular_map, un_specular_exponent);
    }


    fsout_frag_color_0 = vec4(color, 1.0);
    fsout_frag_color_1 = vec4(0.0, 0.0, 0.0, 1.0);
    fsout_frag_color_2 = vec4(0.0, 0.0, 0.0, 1.0);
    fsout_frag_color_3 = vec4(0.0, 0.0, 0.0, 1.0);
}
