#version 460 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;
out vec3 fsin_normal;
out vec2 fsin_texcoords;

out vec3 TBN_viewpos;
out vec3 TBN_fragpos;
out mat3 TBN;
out mat3 TBNT;

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

uniform sampler2D un_heightmap;
uniform float     un_height_scale;
uniform float     un_world_scale;
uniform Material  un_material_0;
uniform Material  un_material_1;


float heightQuery( vec2 worldspace )
{
    vec2 uv = (worldspace / un_world_scale) * 0.5 + 0.5;
    return un_height_scale * textureLod(un_heightmap, uv, 4).r;
}


vec3 compute_normal( vec2 worldspace )
{
    vec3 offset = vec3(2.0, 2.0, 0.0);

    float hl = heightQuery(worldspace - offset.xz);
    float hr = heightQuery(worldspace + offset.xz);
    float hd = heightQuery(worldspace - offset.zy);
    float hu = heightQuery(worldspace + offset.zy);

    return normalize(vec3(hl-hr, hd-hu, 2.0));
}


void main()
{
    vec4 worldpos = un_model * vec4(un_world_scale * vsin_pos, 1.0);
    worldpos.y = heightQuery(worldpos.xz);


    fsin_fragpos = worldpos.xyz;
    fsin_normal  = compute_normal(worldpos.xz);
    fsin_texcoords = vsin_texcoords;

    // vec3 N = normalize(mat3(model) * normalize(vsin_normal));
    // vec3 T = normalize(mat3(model) * normalize(vsin_tangent));
    // T = normalize(T - dot(T, N) * N);
    // vec3 B = cross(N, T);
    // B = normalize(B - dot(B, N) * N);

    // TBN  = mat3(T, B, N);
    // TBNT = transpose(TBN);
    // TBN_fragpos = TBNT * fsin_fragpos;
    // TBN_viewpos = TBNT * un_viewpos;

    gl_Position = un_projection * un_view * worldpos;
}