#version 330 core

#define SQ(a) ((a)*(a))

out vec4 FragColor;
uniform vec2 mouse;

void main()
{
    vec2 screenpos = vec2(gl_FragCoord.x / 1000.0, gl_FragCoord.y / 1000.0);
    vec2 mousepos = vec2(mouse.x, 1000.0 - mouse.y) / 1000.0;
    float dist = sqrt(SQ(screenpos.x - mousepos.x) + SQ(screenpos.y - mousepos.y));

    FragColor = vec4(0.0, dist, 0.0, 1.0);
}
