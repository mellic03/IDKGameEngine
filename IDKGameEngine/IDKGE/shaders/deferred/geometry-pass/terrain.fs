#version 460 core

#extension GL_ARB_bindless_texture: require


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

layout (bindless_sampler) uniform sampler2D un_heightmap;
uniform Material  un_material_0;
uniform Material  un_material_1;


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


// in vec3 TBN_viewpos;
// in vec3 TBN_fragpos;
// in mat3 TBN;
// in mat3 TBNT;


struct DirLight
{
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
};


layout (std140, binding = 5) uniform UBO_dirlights
{
    DirLight    ubo_dirlights[10];
    mat4        un_cascade_matrices[4];
};
uniform sampler2D   un_dirlight_depthmaps[10];


float rand( vec2 seed )
{
    return fract(sin(dot(seed, vec2(12.9898, 78.233))) * 43758.5453);
}



void main()
{
    vec2 texcoords = fsin_texcoords;

    vec3 color_A = texture(un_material_0.albedo, fsin_texcoords).rgb;
    vec3 color_B = texture(un_material_1.albedo, fsin_texcoords).rgb;

    // float alpha = dot(normalize(fsin_normal), vec3(0.0, 1.0, 0.0)) * 0.5 + 0.5;
    //       alpha = smoothstep(0.94, 0.98, alpha);
    //       alpha = clamp(alpha, 0.0, 1.0);

    float alpha = clamp(fsin_fragpos.y / 2.0, 0.0, 1.0);

    vec3 albedo = mix(color_B, color_A, alpha);

    // vec3  ao_rough_metal = texture(un_material.rough_metal, texcoords).rgb;

    float ao = 1.0; // ao_rough_metal.r;
          ao = clamp(ao, 0.0, 1.0);

    float roughness = 0.99; // un_material.roughness_strength; // * ao_rough_metal.g;
          roughness = clamp(roughness, 0.0, 1.0);

    float metallic = 0.0; // un_material.metallic_strength; // * ao_rough_metal.b;
          metallic = clamp(metallic, 0.0, 1.0);

    vec3 N = normalize(fsin_normal);

    fsout_albedo            = vec4(albedo.rgb, 1.0);
    fsout_position_metallic = vec4(fsin_fragpos, metallic);
    fsout_normal_ao         = vec4(N, ao);
    fsout_roughness_ref     = vec4(roughness, 0.0, 0.0, 0.0);
}
