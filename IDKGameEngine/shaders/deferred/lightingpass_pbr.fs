#version 440 core
#define DIRLIGHT_BIAS  0.01
#define DIRSHADOW_AMBIENT  1
#define EPSILON  0.000001
#define MAX_DIRLIGHTS    10
#define MAX_POINTLIGHTS  10
#define MAX_SPOTLIGHTS   10
#define MIPLEVEL_DIFFUSE   2.0
#define MIPLEVEL_SPECULAR  4.0
#define NUM_DIRLIGHTS    0
#define NUM_POINTLIGHTS  0
#define NUM_SPOTLIGHTS   0
#define PI  3.14159265359

layout (location = 0) out vec4 fsout_frag_color;


in vec2 fsin_texcoords;

uniform sampler2D un_texture_0;
uniform sampler2D un_texture_1;
uniform sampler2D un_texture_2;
uniform sampler2D un_texture_3;
uniform samplerCube un_skybox_diffuse;
uniform samplerCube un_skybox_specular;
uniform sampler2D un_BRDF_LUT;

layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
    vec3 un_cam_beg;
};

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
struct DirLight
{
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
};


layout (std140, binding = 5) uniform UBO_dirlights
{
    DirLight    un_dirlights[MAX_DIRLIGHTS];
    mat4        un_cascade_matrices[4];
};

uniform sampler2DArrayShadow   un_dirlight_depthmap;
vec3 fresnelSchlick( float cosTheta, vec3 F0 )
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickR( float cosTheta, vec3 F0, float roughness )
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float NDF( float roughness, vec3 N, vec3 H )
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

float GSF( float roughness, vec3 N, vec3 V, vec3 L )
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 cook_torrance( float roughness, vec3 N, vec3 V, vec3 L, vec3 H, vec3 fresnel )
{
    float ndf = NDF(roughness, N, H);
    float gsf = GSF(roughness, N, V, L);
    float denom = 4.0 * dot(V, N) * dot(L, N);
    return (ndf * gsf * fresnel) / (max(denom, EPSILON));
}
vec3 pointlight_contribution( int idx, vec3 position, vec3 F0, vec3 N, vec3 V, vec3 R,
                              vec3 albedo, float metallic, float roughness, float ao, float emission )
{
    PointLight light = ubo_pointlights[idx];
    vec3  light_position = light.position.xyz;
    vec3  light_ambient  = light.ambient.xyz;
    vec3  light_diffuse  = light.diffuse.xyz;
    vec3  falloff        = light.attenuation.xyz;

    const vec3 L  = normalize(light_position - position);
    const vec3 H  = normalize(V + L);

    float d = distance(light_position, position);
    float attenuation = 1.0 / (falloff.x + d*falloff.y + d*d*falloff.z);
    vec3  radiance    = light_diffuse * attenuation;

    float ndf = NDF(roughness, N, H);
    float G = GSF(roughness, N, V, L);
    vec3  F = fresnelSchlickR(max(dot(H, V), 0.0), F0, roughness);

    vec3  numerator   = ndf * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + EPSILON;
    vec3  specular    = numerator / denominator;

    vec3 Ks = F;
    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);

    return emission*albedo + (Kd * albedo / PI + specular) * radiance * NdotL;
}
vec3 dirlight_contribution( int idx, vec3 position, vec3 F0, vec3 N, vec3 V, vec3 R,
                            vec3 albedo, float metallic, float roughness, float ao, float emission )
{
    DirLight light       = un_dirlights[idx];
    vec3  light_dir      = light.direction.xyz;
    vec3  light_ambient  = light.ambient.xyz;
    vec3  light_diffuse  = light.diffuse.xyz;

    const vec3 L  = normalize(-light_dir);
    const vec3 H  = normalize(V + L);

    vec3 radiance = light_diffuse;

    float ndf = NDF(roughness, N, H);
    float G = GSF(roughness, N, V, L);
    vec3  F = fresnelSchlickR(max(dot(H, V), 0.0), F0, roughness);

    vec3  numerator   = ndf * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + EPSILON;
    vec3  specular    = numerator / denominator;

    vec3 Ks = F;
    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);

    return emission*albedo + (Kd * albedo/PI + specular) * radiance * NdotL;
}
uniform vec4  un_cascade_depths;


#define KERNEL_HW 2

float sampleDepthMap( int layer, vec3 uv, float bias )
{
    vec2 texelSize = 1.0 / textureSize(un_dirlight_depthmap, 0).xy;

    float shadow = 0.0;

    for(int x = -KERNEL_HW; x <= KERNEL_HW; ++x)
    {
        for(int y = -KERNEL_HW; y <= KERNEL_HW; ++y)
        {
            vec2 sample_uv    = uv.xy + vec2(x, y) * texelSize;
            vec4 sample_coord = vec4(sample_uv, float(layer), uv.z);

            shadow -= texture(un_dirlight_depthmap, sample_coord); 
        }    
    }

    return shadow / (KERNEL_HW*KERNEL_HW);
}


float dirlight_shadow( int idx, vec3 position, vec3 N )
{
    vec3 L = normalize(-un_dirlights[idx].direction.xyz);

    vec3  fragpos_viewspace = (un_view * vec4(position, 1.0)).xyz;
    float frag_depth        = abs(fragpos_viewspace.z);

    int layer = 3;
    for (int i=0; i<4; i++)
    {
        if (frag_depth < un_cascade_depths[i])
        {
            layer = i;
            break;
        }
    }

    vec4 fragpos_lightspace = un_cascade_matrices[layer] * vec4(position, 1.0);
    vec3 projCoords = fragpos_lightspace.xyz / fragpos_lightspace.w;
    projCoords = projCoords * 0.5 + 0.5; 

    float bias = DIRLIGHT_BIAS;

    float shadow = sampleDepthMap(layer, projCoords, bias);

    return 1.0 - shadow;
}


vec3 dirlight_contribution_shadowmapped( int idx, vec3 position, vec3 F0, vec3 N, vec3 V, vec3 R,
                                         vec3 albedo, float metallic, float roughness, float ao, float emission )
{
    DirLight light       = un_dirlights[idx];
    vec3  light_dir      = light.direction.xyz;
    vec3  light_ambient  = light.ambient.xyz;
    vec3  light_diffuse  = light.diffuse.xyz;
    float light_strength = light.diffuse.w;

    const vec3 L  = normalize(-light_dir);
    const vec3 H  = normalize(V + L);

    float ndf = NDF(roughness, N, H);
    float G = GSF(roughness, N, V, L);
    vec3  F = fresnelSchlickR(max(dot(H, V), 0.0), F0, roughness);

    vec3  numerator   = ndf * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + EPSILON;
    vec3  specular    = numerator / denominator;

    vec3 Ks = F;
    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);

    float shadow = dirlight_shadow(idx, position, N);

    return shadow * (Kd * albedo/PI + specular) * light_strength * light_diffuse * NdotL;
}

void main()
{
    vec4  albedo_metallic = texture( un_texture_0, fsin_texcoords );
    vec3  albedo       = albedo_metallic.rgb;
    float metallic     = albedo_metallic.a;

    vec4  position_a   = texture( un_texture_1, fsin_texcoords );
    vec3  position     = position_a.xyz;
    vec4  normal_ao    = texture( un_texture_2, fsin_texcoords );
    vec3  normal       = normal_ao.xyz;
    float ao           = normal_ao.w;

    vec4  roughness_ref = texture( un_texture_3, fsin_texcoords);
    float roughness     = roughness_ref.x;
    vec3  F0            = roughness_ref.yzw;
          F0            = (metallic < 0.2) ? vec3(0.04) : F0;
    float emission      = 0.0; 

    if (F0 == vec3(-1.0))
    {
       fsout_frag_color = vec4(albedo, 1.0);
       return;
    }

    vec3 N = normal;
    vec3 V = normalize(un_viewpos - position);
    vec3 R = reflect(-V, N); 

    F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo = vec3(0.0);


    Lo += dirlight_contribution_shadowmapped(0, position, F0, N, V, R, albedo, metallic, roughness, ao, emission);

    vec3 Ks = fresnelSchlick(max(dot(N, V), 0.0), F0);
    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic);

    vec3 irradiance = texture(un_skybox_diffuse, N).rgb;
    vec3 diffuse    = irradiance * albedo;

    vec3 prefilter = textureLod(un_skybox_specular, R, roughness*MIPLEVEL_SPECULAR).rgb;
    vec2 brdf      = texture(un_BRDF_LUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular  = prefilter * (Ks * brdf.x + brdf.y);
    vec3 ambient   = (Kd * diffuse + specular) * ao;

    #if DIRSHADOW_AMBIENT == 0
        float shadow = dirlight_shadow(0, position, N);
              shadow = clamp(shadow, un_dirlights[0].ambient.x, 1.0);
        ambient *= shadow;
    #endif

    vec3 color = ambient + Lo;

    fsout_frag_color = vec4(color, 1.0);
}
