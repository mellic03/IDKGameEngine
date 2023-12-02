#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;
uniform sampler2D un_texture_4;

uniform float intensity;


void main()
{
    vec3 color_0 = texture(un_texture_0, fsin_texcoords).rgb;
    vec3 color_1 = intensity * texture(un_texture_4, fsin_texcoords).rgb;
    vec3 color = color_0 + color_1;

    fsout_frag_color = vec4(color, 1.0);
}