#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

#include "UBO_lightsources.glsl"

in vec2 fsin_texcoords;

uniform sampler2D un_texture_0;
uniform sampler2D un_texture_1;
uniform sampler2D un_texture_2;
uniform sampler2D un_texture_3;

layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};


#include "methods.glsl"

void main()
{
    vec4 albedospec = texture( un_texture_0, fsin_texcoords );
    vec3 albedo     = albedospec.rgb;
    float specular  = albedospec.a;

    vec3 position   = texture( un_texture_1, fsin_texcoords ).xyz;
    vec3 normal     = texture( un_texture_2, fsin_texcoords ).xyz;
    vec3 emission   = texture( un_texture_3, fsin_texcoords ).xyz;

    vec3 view_dir = normalize(un_viewpos - position);

    vec3 color = vec3(0.0);
    // for (int i = 0; i < ubo_num_pointlights; i++)
    // {
    //     color += pointlight_contribution(i, view_dir, albedo, specular, 32.0);
    // }

    for (int i = 0; i < ubo_num_spotlights; i++)
    {
        color += spotlight_contribution(i, view_dir, position, normal, albedo, specular, 32.0);
    }

    for (int i = 0; i < ubo_num_dirlights; i++)
    {
        color += dirlight_contribution(i, view_dir, position, normal, albedo, specular, 32.0);
    }

    // for (int i = 0; i < ubo_num_dirlights; i++)
    // {
    //     color += dirlight_contribution(
    //         i, view_dir, albedo, specular_map,
    //         un_specular_exponent,
    //         fsin_fragpos_dirlightspace[i]
    //     );
    // }

    fsout_frag_color = vec4(color, 1.0);
}
