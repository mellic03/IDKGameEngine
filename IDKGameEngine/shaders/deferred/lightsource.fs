#version 460 core

#define MAX_POINTLIGHTS 10


layout (location = 0) out vec4 fsout_albedo_metallic;
layout (location = 1) out vec4 fsout_position;
layout (location = 2) out vec4 fsout_normal;
layout (location = 3) out vec4 fsout_roughness_ao_em;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec2 fsin_texcoords;

uniform vec4 un_color;

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
    fsout_albedo_metallic = vec4(un_color.rgb, 0.5);
    fsout_position        = vec4(fsin_fragpos, 1.0);
    fsout_normal          = vec4(normalize(fsin_normal), 1.0);
    fsout_roughness_ao_em = vec4(1.0, 1.0, 1.0, 1.0);
}
