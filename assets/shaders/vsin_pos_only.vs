#version 440 core

layout (location = 0) in vec3 vsin_pos;

uniform mat4 un_model;
uniform mat4 un_view;
uniform mat4 un_projection;

void main()
{
    vec4 pos = vec4(vsin_pos, 1.0);
    gl_Position = un_projection * un_view * un_model * pos;
}