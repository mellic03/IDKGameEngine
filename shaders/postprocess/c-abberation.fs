#version 440 core

out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;

uniform vec2 un_r_offset;
uniform vec2 un_g_offset;
uniform vec2 un_b_offset;


void main()
{
    float r = texture( un_texture_0, fsin_texcoords - un_g_offset - un_b_offset ).r;
    float g = texture( un_texture_0, fsin_texcoords - un_r_offset - un_b_offset ).g;
    float b = texture( un_texture_0, fsin_texcoords - un_r_offset - un_g_offset ).b;

    vec3 color = vec3(r, g, b);

    fsout_frag_color = vec4(color, 1.0);
}
