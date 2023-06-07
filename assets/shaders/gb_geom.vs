#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec2 vsin_normal;
layout (location = 2) in vec2 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

uniform mat4 un_model;
uniform mat4 un_view;
uniform mat4 un_projection;

void main()
{
    gl_Position = un_projection * un_view * un_model * vec4(vsin_pos, 1.0);
}
