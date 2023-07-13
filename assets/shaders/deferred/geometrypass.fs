#version 440 core

#define MAX_POINTLIGHTS 10

layout (location = 0) out vec4 fsout_albedospec;
layout (location = 1) out vec4 fsout_position;
layout (location = 2) out vec4 fsout_normal;
layout (location = 3) out vec4 fsout_emission;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec2 fsin_texcoords;
in vec4 fsin_fragpos_dirlightspace[10];

uniform sampler2D un_albedo_texture;
uniform sampler2D un_specular_texture;
uniform sampler2D un_normal_texture;
uniform float un_specular_exponent;

layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};


void main()
{
    vec3 albedo_map = texture(un_albedo_texture, fsin_texcoords).rgb;
    float specular_map = texture(un_specular_texture, fsin_texcoords).r;

    fsout_albedospec = vec4(albedo_map, specular_map);
    fsout_position   = vec4(fsin_fragpos, 1.0);
    fsout_normal     = vec4(fsin_normal, 0.0);
    fsout_emission   = vec4(0.0);
}
