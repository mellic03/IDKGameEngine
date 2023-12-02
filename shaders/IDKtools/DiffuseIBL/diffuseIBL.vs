#version 330 core

layout (location = 0) in vec3 vsin_pos;

out vec3 fsin_fragpos;

uniform mat4 un_projection;
uniform mat4 un_view;

void main()
{
    fsin_fragpos = vsin_pos;
    gl_Position = un_projection * un_view * vec4(fsin_fragpos, 1.0);
}
