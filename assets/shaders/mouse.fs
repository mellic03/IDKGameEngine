#version 440 core

#define SQ(a) ((a)*(a))

layout (location = 0) out vec4 FragColor1;
layout (location = 1) out vec4 FragColor2;
layout (location = 2) out vec4 FragColor3;
layout (location = 3) out vec4 FragColor4;
uniform vec2 mouse;

void main()
{
    vec2 screenpos = vec2(gl_FragCoord.x / 1000.0, gl_FragCoord.y / 1000.0);
    vec2 mousepos = vec2(mouse.x, 1000.0 - mouse.y) / 1000.0;
    float dist = sqrt(SQ(screenpos.x - mousepos.x) + SQ(screenpos.y - mousepos.y));

    FragColor1 = vec4(0.0, dist, 0.0, 1.0);
    FragColor2 = vec4(0.0, dist, 0.0, 1.0);
    FragColor3 = vec4(0.0, dist, 0.0, 1.0);
    FragColor4 = vec4(0.0, dist, 0.0, 1.0);
}
