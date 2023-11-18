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

    vec3 color = vec3(albedo);


    fsout_frag_color = vec4(color, 1.0);
}
