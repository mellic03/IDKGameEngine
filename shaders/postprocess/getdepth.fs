#version 440 core

layout (location = 0) out vec4 fsout_frag_color;
layout (location = 1) out vec4 fsout_frag_depth;

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


void main()
{
    vec3 position = texture(un_texture_1, fsin_texcoords).xyz;
    fsout_frag_depth = vec4(vec3(distance(un_viewpos, position)), 1.0);
}