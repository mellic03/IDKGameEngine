#version 440 core

layout (location = 0) out vec4 fsout_frag_color;
layout (location = 1) out float fsout_depth;

in vec2 fsin_texcoords;


layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};


void main()
{
    vec3 color = vec3(fsin_texcoords, 0.0);
    vec3 depth = vec3(fsin_texcoords, 0.0);

    fsout_frag_color = vec4(color, 1.0);
    fsout_depth = fsin_texcoords.x;
}