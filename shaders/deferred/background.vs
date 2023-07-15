#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;

uniform mat4 un_model;

layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};


void main()
{
    vec4 worldpos = inverse(un_view) * un_model * vec4(vsin_pos, 1.0);
    fsin_fragpos = worldpos.xyz;
    gl_Position = un_projection * un_view * worldpos;
}
