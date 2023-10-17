#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec2 vsin_texcoords;

out vec2 fsin_texcoords;


void main()
{
    const vec2 position  = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;
    const vec2 texcoords = (position + 1) * 0.5;

    fsin_texcoords = texcoords;
    gl_Position = vec4(position, 1.0, 1.0);
}


