#version 440 core

layout (location = 0) out vec4 fsout_albedo;

in vec4 fsin_fragpos;
uniform samplerCube un_skybox;

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
    fsout_albedo = texture(un_skybox, fsin_fragpos.xyz - un_viewpos);
}
