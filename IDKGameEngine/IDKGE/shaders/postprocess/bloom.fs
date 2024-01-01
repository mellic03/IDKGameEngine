#version 460 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;


void main()
{
    vec3 color = texture(un_texture_0, fsin_texcoords).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    fsout_frag_color = vec4(min(color*color, vec3(1.0)), 1.0);
}