#version 440 core

layout (location = 0) out vec4 fsout_frag_color;
in vec2 vsout_texcoords;
uniform sampler2D un_screen_texture;

void main()
{
    // fsout_frag_color = vec4(vsout_texcoords.xy, 0.0, 1.0);
    fsout_frag_color = texture(un_screen_texture, vsout_texcoords);
}
