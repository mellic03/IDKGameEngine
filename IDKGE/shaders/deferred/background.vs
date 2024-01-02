#version 460 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec2 vsin_texcoords;

out vec4 fsin_fragpos;

uniform mat4 un_model;

struct Camera
{
    vec4 position;
    vec4 beg;
    vec4 aberration_rg;
    vec4 aberration_b;
    vec4 exposure;
};

layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
    Camera un_camera;
};


void main()
{
    vec4 worldpos = inverse(un_view) * un_model * vec4(vsin_pos, 1.0);
    fsin_fragpos = worldpos;
    gl_Position = un_projection * un_view * worldpos;
}
