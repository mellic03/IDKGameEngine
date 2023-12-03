#version 440 core

layout (location = 0) out vec4 fsout_albedo_metallic;
layout (location = 1) out vec4 fsout_position;
layout (location = 2) out vec4 fsout_normal_ao;
layout (location = 3) out vec4 fsout_roughness_ref;

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
    fsout_albedo_metallic = texture(un_skybox, fsin_fragpos.xyz - un_viewpos);
    fsout_position.xyz = fsin_fragpos.xyz;
    fsout_position.w   = 1.0;

    fsout_normal_ao     = vec4(1.0);
    fsout_roughness_ref = vec4(1.0, vec3(-1.0));
}
