#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;
uniform sampler2D un_texture_1;


void main()
{
    vec3 color_0 = texture(un_texture_0, fsin_texcoords).rgb;
    vec3 color_1 = texture(un_texture_1, fsin_texcoords).rgb;

    fsout_frag_color = vec4(color_0, 1.0) + vec4(color_1, 1.0);
}

