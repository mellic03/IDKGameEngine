#version 440 core
layout (location = 0) in vec3 vsin_pos;

uniform mat4 un_dirlight_projection;
uniform mat4 un_dirlight_view;
uniform mat4 un_model;

void main()
{
    gl_Position = un_dirlight_projection * un_dirlight_view * un_model * vec4(vsin_pos, 1.0);
}  



