#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;
out vec3 fsin_normal;
out vec2 fsin_texcoords;

uniform mat4 un_model;
uniform mat4 un_view;
uniform mat4 un_projection;

void main()
{
    fsin_fragpos = (un_model * vec4(vsin_pos, 1.0)).xyz;
    fsin_normal = (un_model * vec4(vsin_normal, 0.0)).xyz;
    fsin_texcoords = vsin_texcoords;

    gl_Position = un_projection * un_view * un_model * vec4(vsin_pos, 1.0);
}
