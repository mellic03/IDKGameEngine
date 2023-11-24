#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;


void main()
{
    fsout_frag_color = texture(un_texture_0, fsin_texcoords);
}
