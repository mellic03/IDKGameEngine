#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec2 vsin_normal;
layout (location = 2) in vec2 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

uniform mat4 model;

void main()
{
    vec4 pos = vec4(vsin_pos, 1.0);
    gl_Position = model * pos;
}
