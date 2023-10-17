#version 440 core

layout (location = 0) out vec4 FragColor;

uniform vec3 un_color;

void main()
{
    FragColor = vec4(un_color, 1.0);
}
