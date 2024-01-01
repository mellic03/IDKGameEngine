#version 460 core

#extension GL_ARB_bindless_texture: require
layout (std430, binding = 8) buffer SBO_bindless_textures
{
    sampler2D un_bindless_samplers[128];
};
uniform int un_material_id;


layout (location = 0) out vec4 fsout_albedo;
layout (location = 1) out vec4 fsout_position_metallic;
layout (location = 2) out vec4 fsout_normal_ao;
layout (location = 3) out vec4 fsout_roughness_ref;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec2 fsin_texcoords;


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
    vec2 texcoords = fsin_texcoords;

    int albedo_idx = 3*un_material_id + 0;
    int normal_idx = 3*un_material_id + 1;
    int ao_r_m_idx = 3*un_material_id + 2;

    vec4 albedo = texture(un_bindless_samplers[albedo_idx], texcoords).rgba;
    vec3 ao_r_m = texture(un_bindless_samplers[ao_r_m_idx], texcoords).rgb;
    vec3 normal = texture(un_bindless_samplers[normal_idx], texcoords).xyz * 2.0 - 1.0;

    if (albedo.a < 0.05)
        discard;


    float roughness = ao_r_m.g;
          roughness = clamp(roughness, 0.05, 0.95);

    float metallic = 0.0;
          metallic = clamp(metallic, 0.0, 1.0);

    vec3 N = normalize(fsin_normal);

    fsout_albedo            = vec4(albedo.rgb, 1.0);
    fsout_position_metallic = vec4(fsin_fragpos, metallic);
    fsout_normal_ao         = vec4(N, 1.0);
    fsout_roughness_ref     = vec4(1.0, vec3(-2.0));
}
