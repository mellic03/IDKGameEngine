#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec2 vsin_texcoords;

out vec2 fsin_texcoords;

void main()
{
    fsin_texcoords = vsin_texcoords;
    gl_Position = vec4(vsin_pos, 1.0);
}
