#version 330 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;
out vec2 fsin_texcoords;

uniform mat4 un_projection;
uniform mat4 un_view;
uniform mat4 un_model;

void main()
{
    fsin_fragpos = vsin_pos + vec3(0.0, -2.0, 0.0);
    fsin_texcoords = vsin_texcoords;

    gl_Position = un_projection * un_view * vec4(vsin_pos + vec3(0.0, -2.0, 0.0), 1.0);
}
