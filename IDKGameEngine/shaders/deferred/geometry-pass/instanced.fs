#version 460 core

#define MAX_POINTLIGHTS 10


layout (location = 0) out vec4 fsout_albedo;
layout (location = 1) out vec4 fsout_position_metallic;
layout (location = 2) out vec4 fsout_normal_ao;
layout (location = 3) out vec4 fsout_roughness_ref;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec2 fsin_texcoords;


struct Material
{
    sampler2D albedo;
    sampler2D rough_metal;
    sampler2D ao;
    sampler2D displacement;
    sampler2D normal;

    float metallic_strength;
    float roughness_strength;
    float displacement_strength;
    float normal_strength;
};
uniform Material un_material;


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

    vec4 albedo = texture( un_material.albedo, texcoords ).rgba;
    if (albedo.a < 0.05)
        discard;

    vec3  ao_rough_metal = texture(un_material.rough_metal, texcoords).rgb;

    // float ao = ao_rough_metal.r;
    //       ao = clamp(ao, 0.0, 1.0);

    float roughness = un_material.roughness_strength * ao_rough_metal.g;
          roughness = clamp(roughness, 0.05, 0.95);

    float metallic = 0.0; // un_material.metallic_strength; // * ao_rough_metal.b;
          metallic = clamp(metallic, 0.0, 1.0);

    vec3 normal = normalize(fsin_normal);

    fsout_albedo            = vec4(albedo.rgb, 1.0);
    fsout_position_metallic = vec4(fsin_fragpos, metallic);
    fsout_normal_ao         = vec4(normal, 1.0);
    fsout_roughness_ref     = vec4(1.0, vec3(-2.0));
}
