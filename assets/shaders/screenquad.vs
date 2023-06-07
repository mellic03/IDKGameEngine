#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 2) in vec2 vsin_texcoords;

out vec2 vsout_texcoords;

void main()
{
    vsout_texcoords = vsin_texcoords;
    gl_Position = vec4(vsin_pos, 1.0);
}

