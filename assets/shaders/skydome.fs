#version 440 core

out vec4 fsout_frag_color;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec2 fsin_texcoords;

uniform sampler2D un_albedo_texture;

#include "UBOs/UBOs.glsl"

void main()
{
    vec3 albedo_map = texture(un_albedo_texture, fsin_texcoords).rgb;
    fsout_frag_color = vec4(albedo_map, 1.0);
}
