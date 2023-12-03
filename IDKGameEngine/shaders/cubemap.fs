#version 330 core

out vec4 fsout_fragcolor;

in vec3 fsin_fragpos;
in vec2 fsin_texcoords;

uniform sampler2D un_albedo_texture;


void main()
{
    float d = distance(vec3(0.0), fsin_fragpos);

    fsout_fragcolor = (1.0 / (d*d)) * texture(un_albedo_texture, fsin_texcoords);
}
