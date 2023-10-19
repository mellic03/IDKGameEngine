#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;
uniform sampler2D un_texture_1;

uniform sampler2D un_texture_4;
uniform sampler2D un_texture_5;


void main()
{
    vec3  color_0 = texture(un_texture_0, fsin_texcoords).rgb;
    float depth_0 = texture(un_texture_1, fsin_texcoords).r;

    vec3  color_1 = texture(un_texture_4, fsin_texcoords).rgb;
    float depth_1 = texture(un_texture_5, fsin_texcoords).r;


    if (depth_0 < depth_1)
    {
        fsout_frag_color = vec4(color_0, 1.0);
    }

    else
    {
        fsout_frag_color = vec4(color_1, 1.0);
    }
}