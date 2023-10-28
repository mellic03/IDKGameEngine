#version 440 core

layout (location = 0) out vec4 fsout_albedospec;
layout (location = 1) out vec4 fsout_position;
layout (location = 2) out vec4 fsout_normal;
layout (location = 3) out vec4 fsout_emission;

in vec4 fsin_fragpos;

void main()
{
    fsout_albedospec = vec4(0.0, 0.0, 0.0, 1.0);
    fsout_position   = fsin_fragpos;
    fsout_normal     = vec4(1.0);
    fsout_emission   = vec4(0.0);
}
