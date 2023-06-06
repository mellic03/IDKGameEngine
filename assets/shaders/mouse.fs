#version 330 core

#include "testlib.glsl"

out vec4 FragColor;
uniform int screen_x, screen_y;
uniform vec2 mouse;

void main()
{
    vec2 screenpos = vec2(gl_FragCoord.x / screen_x, gl_FragCoord.y / screen_y);
    vec2 mousepos = mouse / 1000.0;
    float dist = sqrt(SQ(screenpos.x - mousepos.x) + SQ(screenpos.y - mousepos.y));

    FragColor = vec4(dist, 0.0, 0.0, 1.0);
}
