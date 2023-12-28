#version 460 core

out vec2 fsin_texcoords;

void main()
{
    const vec2 position  = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;
    const vec2 texcoords = (position + 1) * 0.5;

    fsin_texcoords = texcoords;
    gl_Position = vec4(position, 1.0, 1.0);
}


