#version 440 core
#define BIAS 0.0015

out vec4 color;

void main()
{
    gl_FragDepth = gl_FragCoord.z;
    gl_FragDepth += gl_FrontFacing ? BIAS : 0.0;

    color = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
}
