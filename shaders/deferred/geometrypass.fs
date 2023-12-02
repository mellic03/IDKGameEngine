#version 440 core

#define MAX_POINTLIGHTS 10


layout (location = 0) out vec4 fsout_albedo_metallic;
layout (location = 1) out vec4 fsout_position;
layout (location = 2) out vec4 fsout_normal_ao;
layout (location = 3) out vec4 fsout_roughness_ref;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec2 fsin_texcoords;


#define NONMETAL  0
#define IRON      1
#define COPPER    2
#define GOLD      3
#define ALUMINIUM 4

vec3 get_reflectance( int reflectance )
{
    switch (reflectance)
    {
        case NONMETAL:  return vec3(0.04);
        case IRON:      return vec3(0.56, 0.57, 0.58);
        case COPPER:    return vec3(0.95, 0.64, 0.54);
        case GOLD:      return vec3(1.0,  0.71, 0.29);
        case ALUMINIUM: return vec3(0.91, 0.92, 0.92);
    }
}

struct Material
{
    sampler2D albedo;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D ao;
    sampler2D displacement;
    sampler2D normal;

    float metallic_strength;
    float roughness_strength;
    float displacement_strength;
    float normal_strength;

    int reflectance;
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


in vec3 TBN_viewpos;
in vec3 TBN_fragpos;
in mat3 TBN;
in mat3 TBNT;


#define PARALLAX 0
#define parallax_steps 8.0


struct vec5
{
    vec2 uv;
    vec3 fragpos;
};


struct DirLight
{
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
};


layout (std140, binding = 5) uniform UBO_dirlights
{
    DirLight    ubo_dirlights[10];
    mat4        ubo_dirlight_matrices[10];
};
uniform sampler2D   un_dirlight_depthmaps[10];



vec2 parallax_map()
{
    const float strength = un_material.displacement_strength;

    vec3 viewdir = normalize(TBNT*un_viewpos - TBNT*fsin_fragpos);
    viewdir.y *= -1.0;

    vec2 p = strength * (viewdir.xy / viewdir.z);

    const float layer_height = 1.0 / parallax_steps;
    vec2 delta = p / parallax_steps;

    vec2  prev_uv = fsin_texcoords;
    vec2  uv = fsin_texcoords;
    float texture_depth = texture(un_material.displacement, uv).r;
    float prev_texture_depth = texture_depth;
    float layer_depth   = 0.0;

    while (layer_depth < texture_depth)
    {
        prev_uv = uv;
        uv += delta;

        prev_texture_depth = texture_depth;
        texture_depth = texture(un_material.displacement, uv).r;

        layer_depth += layer_height;
    }

    float afterDepth  = texture_depth - layer_depth;
    float beforeDepth = prev_texture_depth - layer_depth + layer_height;
    float a = afterDepth / (afterDepth - beforeDepth);

    vec2 final_uv = a*prev_uv + (1.0 - a)*uv;

    return final_uv;
}



void main()
{
    #if PARALLAX == 1
        vec2 texcoords = parallax_map();
    #else
        vec2 texcoords = fsin_texcoords;
    #endif

    vec3  albedo    = texture( un_material.albedo, texcoords ).rgb;

    float metallic  = un_material.metallic_strength  * texture(un_material.metallic, texcoords).r;
          metallic  = clamp(metallic, 0.0, 1.0);

    float roughness = un_material.roughness_strength * texture(un_material.roughness, texcoords).r;
          roughness = clamp(roughness, 0.0, 1.0);

    float ao        = texture( un_material.ao, texcoords ).r;

    float a      = un_material.normal_strength;
    vec3  normal = normalize(TBN * (texture(un_material.normal, texcoords).xyz * 2.0 - 1.0));
          normal = normalize(a*normal + (1.0 - a)*normalize(fsin_normal));


    fsout_albedo_metallic = vec4(albedo, metallic);
    fsout_position        = vec4(fsin_fragpos, 1.0);
    fsout_normal_ao       = vec4(normal, ao);
    fsout_roughness_ref   = vec4(roughness, get_reflectance(un_material.reflectance));
}
