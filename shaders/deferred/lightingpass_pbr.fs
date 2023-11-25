#version 440 core

layout (location = 0) out vec4 fsout_frag_color;


#define MAX_POINTLIGHTS 10
#define MAX_SPOTLIGHTS  10
#define MAX_DIRLIGHTS   10

#define NUM_POINTLIGHTS 0
#define NUM_SPOTLIGHTS  0
#define NUM_DIRLIGHTS   0

#define PI 3.14159265359


in vec2 fsin_texcoords;

uniform sampler2D un_texture_0; // albedo_metallic
uniform sampler2D un_texture_1; // position
uniform sampler2D un_texture_2; // normal
uniform sampler2D un_texture_3; // roughness


layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};

struct DirLight
{
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
};
layout (std140, binding = 5) uniform UBO_dirlights
{
    DirLight    un_dirlights[MAX_DIRLIGHTS];
    mat4        un_dirlight_matrices[MAX_DIRLIGHTS];
};
uniform sampler2D   un_dirlight_depthmaps[MAX_DIRLIGHTS];


struct PointLight
{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 attenuation;
};
layout (std140, binding = 3) uniform UBO_pointlights
{
    PointLight  ubo_pointlights[MAX_POINTLIGHTS];
};


vec3 lambert_diffuse( vec3 albedo, vec3 normal, vec3 incoming )
{
    return (albedo / 3.14159) * dot(normal, incoming);
}


float NDF_GGX( vec3 normal, vec3 halfnormal, float roughness )
{
    const float alpha = roughness*roughness;

    const float A = dot(normal, halfnormal);
    const float B = (A*A) * (alpha*alpha - 1.0) + 1.0;

    return alpha / ( PI * B*B );
}


float schlick_GGX( vec3 normal, vec3 halfnormal, vec3 viewdir, float roughness )
{
    const float a   = roughness*roughness + 1.0;
    const float K   = (a*a) / 8.0;
    const float dnv = max(dot(normal, viewdir), 0.0);

    return dnv / (dnv * (1.0 - K) + K);
}


vec3 fresnel( vec3 halfnormal, vec3 viewdir, vec3 reflectivity )
{
    float dvh = 1.0 - max(dot(viewdir, halfnormal), 0.0);
    dvh = dvh * dvh * dvh * dvh * dvh;
    return reflectivity + (1.0 - reflectivity) * dvh;
}


vec3 specular_BDRF( vec3 normal, vec3 halfnormal, vec3 lightdir, vec3 viewdir, float roughness )
{
    const vec3 reflectivity = vec3(1.0, 0.86, 0.57);

    const float D = NDF_GGX(normal, halfnormal, roughness);
    const float G = schlick_GGX(normal, halfnormal, viewdir, roughness);
    const vec3  F = fresnel(halfnormal, viewdir, reflectivity);

    return (D * G * F) / (dot(normal, lightdir) * dot(normal, viewdir));
}



float dirlight_shadow(int idx, vec3 position)
{
    vec4 fragpos_lightspace = un_dirlight_matrices[idx] * vec4(position, 1.0);

    vec3 projCoords = fragpos_lightspace.xyz / fragpos_lightspace.w;
    projCoords = projCoords * 0.5 + 0.5; 

    float bias = 0.001;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(un_dirlight_depthmaps[idx], 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(un_dirlight_depthmaps[idx], projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return 1.0 - shadow;
}


vec3 dirlight_contribution( int idx, vec3 view_dir, vec3 position, vec3 normal,
                            vec3 albedo, float metallic, float roughness, float ao )
{
    DirLight light = un_dirlights[idx];

    vec3 light_direction = normalize(light.direction.xyz);
    vec3 light_ambient = light.ambient.xyz;
    vec3 light_diffuse = light.diffuse.xyz;

    vec3 frag_to_light = normalize(-light_direction);
    float diffuse_ff = max(dot(normal, frag_to_light), 0.0);
    float diffuse_f = dot(normal, frag_to_light);
    diffuse_f = (diffuse_f + 1.0) / 2.0;

    vec3 halfway_dir = normalize(frag_to_light + view_dir);
    float specular_f = 0.0; // pow(max(dot(normal, halfway_dir), 0.0), spec_exponent);

    const vec3 reflectivity = vec3(1.0, 0.86, 0.57);
    vec3 kS = fresnel(halfway_dir, view_dir, reflectivity);
    vec3 kD = 1.0 - kS;

    vec3 ambient  = albedo * light_ambient;
    vec3 diffuse  = albedo * kD * kS / PI;
    vec3 specular = specular_BDRF(normal, halfway_dir, frag_to_light, view_dir, roughness);

    return ambient + light_diffuse * (diffuse + specular) * dot(normal, frag_to_light);
}




vec3 pointlight_contribution( int idx, vec3 view_dir, vec3 position, vec3 normal,
                              vec3 albedo, float metallic, float roughness, float ao )
{
    vec3 light_position = ubo_pointlights[idx].position.xyz;
    vec3 light_ambient = ubo_pointlights[idx].ambient.xyz;
    vec3 light_diffuse = ubo_pointlights[idx].diffuse.xyz;

    float attenuation_constant = ubo_pointlights[idx].attenuation.x;
    float attentuation_linear = ubo_pointlights[idx].attenuation.y;
    float attentuation_quadratic = ubo_pointlights[idx].attenuation.z;


    float d = distance(position, light_position);
    float attenuation = max(1.0 / (d*d), 0.00001);

    vec3 frag_to_light = normalize(light_position - position);
    float diffuse_f = max(dot(normal, frag_to_light), 0.0);
    vec3 halfway_dir = normalize(frag_to_light + view_dir);

    const vec3 reflectivity = vec3(1.0, 0.86, 0.57);
    vec3 kS = fresnel(halfway_dir, view_dir, reflectivity);
    vec3 kD = 1.0 - kS;

    vec3 ambient  = attenuation * albedo * light_ambient;
    vec3 diffuse  = attenuation * albedo * kD * kS / PI;
    vec3 specular = attenuation * specular_BDRF(normal, halfway_dir, frag_to_light, view_dir, roughness);

    vec3 result = ambient + light_diffuse * (diffuse + specular) * dot(normal, frag_to_light);

    return result;
}

void main()
{
    vec4  albedo_metallic = texture( un_texture_0, fsin_texcoords );
    vec3  albedo      = albedo_metallic.rgb;
    float metallic    = albedo_metallic.a;

    vec3  position    = texture( un_texture_1, fsin_texcoords ).xyz;
    vec3  normal      = texture( un_texture_2, fsin_texcoords ).xyz;

    vec2 roughness_ao = texture( un_texture_3, fsin_texcoords).rg;
    float roughness   = roughness_ao.r;
    float ao          = roughness_ao.g;

    vec3  view_dir   = normalize(un_viewpos - position);

    vec3 color = vec3(0.0);


    for (int i=0; i<NUM_POINTLIGHTS; i++)
    {
        color += pointlight_contribution(
            i,      view_dir, position,  normal,
            albedo, metallic, roughness, ao
        );
    }


    for (int i=0; i<NUM_DIRLIGHTS; i++)
    {
        color += dirlight_contribution(
            i,      view_dir, position,  normal,
            albedo, metallic, roughness, ao
        );
    }

    fsout_frag_color = vec4(color, 1.0);
}
