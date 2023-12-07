#pragma once

#include <string>


namespace idk_shadergen
{


const std::string shader_screenquad =
"#version 440 core\n"
"\n"
"out vec2 fsin_texcoords;\n"
"\n"
"void main()\n"
"{\n"
"    const vec2 position  = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;\n"
"    const vec2 texcoords = (position + 1) * 0.5;\n"
"\n"
"    fsin_texcoords = texcoords;\n"
"\n"
"    gl_Position = vec4(position, 1.0, 1.0);\n"
"}\n";



const std::string shader_begin_header =
"#version 440 core\n"
"\n"
"layout (location = 0) out vec4 fsout_frag_color;\n"
"\n"
"#define MAX_POINTLIGHTS 10\n"
"#define MAX_SPOTLIGHTS  10\n"
"#define MAX_DIRLIGHTS   10\n"
"\n"
"#define NUM_POINTLIGHTS 0\n"
"#define NUM_SPOTLIGHTS  0\n"
"#define NUM_DIRLIGHTS   0\n"
"\n"
"#define PI 3.14159265359\n"
"#define EPSILON 0.000001\n"
"#define MIPLEVEL_DIFFUSE  2.0\n"
"#define MIPLEVEL_SPECULAR 4.0\n"
"\n"
"in vec2 fsin_texcoords;\n"
"\n"
"uniform sampler2D un_texture_0;\n"
"uniform sampler2D un_texture_1;\n"
"uniform sampler2D un_texture_2;\n"
"uniform sampler2D un_texture_3;\n"
"uniform samplerCube un_skybox_diffuse;\n"
"uniform samplerCube un_skybox_specular;\n"
"uniform sampler2D un_BRDF_LUT;\n"
"\n"
"layout (std140, binding = 2) uniform UBO_camera_data\n"
"{\n"
"    mat4 un_view;\n"
"    mat4 un_projection;\n"
"    vec3 un_viewpos;\n"
"    vec3 un_cam_beg;\n"
"};\n"
"\n";


const std::string shader_end_header =
"vec3 fresnelSchlick( float cosTheta, vec3 F0 )\n"
"{\n"
"    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);\n"
"}\n"
"\n"
"vec3 fresnelSchlickR( float cosTheta, vec3 F0, float roughness )\n"
"{\n"
"    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);\n"
"}\n"
"\n"
"float NDF( float roughness, vec3 N, vec3 H )\n"
"{\n"
"    float a = roughness*roughness;\n"
"    float a2 = a*a;\n"
"    float NdotH = max(dot(N, H), 0.0);\n"
"    float NdotH2 = NdotH*NdotH;\n"
"\n"
"    float nom   = a2;\n"
"    float denom = (NdotH2 * (a2 - 1.0) + 1.0);\n"
"    denom = PI * denom * denom;\n"
"\n"
"    return nom / denom;\n"
"}\n"
"\n"
"float GeometrySchlickGGX(float NdotV, float roughness)\n"
"{\n"
"    float r = (roughness + 1.0);\n"
"    float k = (r*r) / 8.0;\n"
"    float nom   = NdotV;\n"
"    float denom = NdotV * (1.0 - k) + k;\n"
"    return nom / denom;\n"
"}\n"
"\n"
"float GSF( float roughness, vec3 N, vec3 V, vec3 L )\n"
"{\n"
"    float NdotV = max(dot(N, V), 0.0);\n"
"    float NdotL = max(dot(N, L), 0.0);\n"
"    float ggx2 = GeometrySchlickGGX(NdotV, roughness);\n"
"    float ggx1 = GeometrySchlickGGX(NdotL, roughness);\n"
"\n"
"    return ggx1 * ggx2;\n"
"}\n"
"\n"
"vec3 cook_torrance( float roughness, vec3 N, vec3 V, vec3 L, vec3 H, vec3 fresnel )\n"
"{\n"
"    float ndf = NDF(roughness, N, H);\n"
"    float gsf = GSF(roughness, N, V, L);\n"
"    float denom = 4.0 * dot(V, N) * dot(L, N);\n"
"    return (ndf * gsf * fresnel) / (max(denom, EPSILON));\n"
"}\n";


const std::string shader_begin_main =
"#define DIRSHADOW_AMBIENT 1\n"
"\n"
"void main()\n"
"{\n"
"    vec4  albedo_metallic = texture( un_texture_0, fsin_texcoords );\n"
"    vec3  albedo       = albedo_metallic.rgb;\n"
"    float metallic     = albedo_metallic.a;\n"
"\n"
"    vec4  position_a   = texture( un_texture_1, fsin_texcoords );\n"
"    vec3  position     = position_a.xyz;\n"
"    vec4  normal_a     = texture( un_texture_2, fsin_texcoords );\n"
"    vec3  normal       = normal_a.xyz;\n"
"    float ao           = normal_a.w;\n"
"\n"
"    vec4  roughness_ref = texture( un_texture_3, fsin_texcoords);\n"
"    float roughness     = roughness_ref.x;\n"
"    vec3  F0            = roughness_ref.yzw;\n"
"          F0            = (metallic < 0.2) ? vec3(0.04) : F0;\n"
"    float emission      = 0.0; // roughness_ref.b;\n"
"\n"
"    if (F0 == vec3(-1.0))\n"
"    {\n"
"       fsout_frag_color = vec4(albedo, 1.0);\n"
"       return;\n"
"    }\n"
"\n"
"    vec3 N = normal;\n"
"    vec3 V = normalize(un_viewpos - position);\n"
"    vec3 R = reflect(-V, N); \n"
"\n"
"    F0 = mix(F0, albedo, metallic);\n"
"\n"
"    vec3 Lo = vec3(0.0);\n"

"\n\n";


const std::string shader_end_main =
"\n"
"    vec3 Ks = fresnelSchlick(max(dot(N, V), 0.0), F0);\n"
"    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic);\n"
"\n"
"    vec3 irradiance = texture(un_skybox_diffuse, N).rgb;\n"
"    vec3 diffuse    = irradiance * albedo;\n"
"\n"
"    vec3 prefilter = textureLod(un_skybox_specular, R, roughness*MIPLEVEL_SPECULAR).rgb;\n"
"    vec2 brdf      = texture(un_BRDF_LUT, vec2(max(dot(N, V), 0.0), roughness)).rg;\n"
"    vec3 specular  = prefilter * (Ks * brdf.x + brdf.y);\n"
"    vec3 ambient   = (Kd * diffuse + specular) * ao;\n"
"\n"
"    #if DIRSHADOW_AMBIENT == 1\n"
"        float shadow = dirlight_shadow(0, position);\n"
"              shadow = clamp(shadow, 0.025, 1.0);\n"
"        ambient *= shadow;\n"
"    #endif\n"
"\n"
"    vec3 color = ambient + Lo;\n"
"\n"
"    fsout_frag_color = vec4(color, 1.0);\n"
"}\n";


const std::string pointlight_definition =
"struct PointLight\n"
"{\n"
"    vec4 position;\n"
"    vec4 ambient;\n"
"    vec4 diffuse;\n"
"    vec4 attenuation;\n"
"};\n"
"\n"
"layout (std140, binding = 3) uniform UBO_pointlights\n"
"{\n"
"    PointLight  ubo_pointlights[MAX_POINTLIGHTS];\n"
"};\n";



const std::string dirlight_definition =
"struct DirLight\n"
"{\n"
"    vec4 direction;\n"
"    vec4 ambient;\n"
"    vec4 diffuse;\n"
"};\n"
"\n"
"layout (std140, binding = 5) uniform UBO_dirlights\n"
"{\n"
"    DirLight    un_dirlights[MAX_DIRLIGHTS];\n"
"    mat4        un_dirlight_matrices[MAX_DIRLIGHTS];\n"
"};\n"
"\n"
"uniform sampler2D   un_dirlight_depthmaps[MAX_DIRLIGHTS];\n";



const std::string pointlight_call_begin =
"    Lo += pointlight_contribution(";

const std::string pointlight_shadowmapped_call_begin =
"    Lo += pointlight_contribution_shadowmapped(";

const std::string dirlight_call_begin =
"    Lo += dirlight_contribution(";

const std::string dirlight_shadowmapped_call_begin =
"    Lo += dirlight_contribution_shadowmapped(";

const std::string light_call_end =
", position, F0, N, V, R, albedo, metallic, roughness, ao, emission);\n";


const std::string dirlight_contribution_shadowmapped =
"#define DIRLIGHT_BIAS 0.0015\n"
"\n"
"float dirlight_shadow( int idx, vec3 position )\n"
"{\n"
"    vec4 fragpos_lightspace = un_dirlight_matrices[idx] * vec4(position, 1.0);\n"
"    vec3 projCoords = fragpos_lightspace.xyz / fragpos_lightspace.w;\n"
"\n"
"    projCoords = projCoords * 0.5 + 0.5; \n"
"    float bias = DIRLIGHT_BIAS;\n"
"    float shadow = 0.0;\n"
"    vec2 texelSize = 0.5 / textureSize(un_dirlight_depthmaps[idx], 0);\n"
"\n"
"    for(int x = -2; x <= 2; ++x)\n"
"    {\n"
"        for(int y = -2; y <= 2; ++y)\n"
"        {\n"
"            float pcfDepth = texture(un_dirlight_depthmaps[idx], projCoords.xy + vec2(x, y) * texelSize).r; \n"
"            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;        \n"
"        }    \n"
"    }\n"
"\n"
"    shadow /= 25.0;\n"
"    return 1.0 - shadow;\n"
"}\n"
"\n"
"vec3 dirlight_contribution_shadowmapped( int idx, vec3 position, vec3 F0, vec3 N, vec3 V, vec3 R,\n"
"                                         vec3 albedo, float metallic, float roughness, float ao, float emission )\n"
"{\n"
"    DirLight light       = un_dirlights[idx];\n"
"    vec3  light_dir      = light.direction.xyz;\n"
"    vec3  light_ambient  = light.ambient.xyz;\n"
"    vec3  light_diffuse  = light.diffuse.xyz;\n"
"    float light_strength = light.diffuse.w;\n"
"\n"
"    const vec3 L  = normalize(-light_dir);\n"
"    const vec3 H  = normalize(V + L);\n"
"\n"
"    float ndf = NDF(roughness, N, H);\n"
"    float G = GSF(roughness, N, V, L);\n"
"    vec3  F = fresnelSchlickR(max(dot(H, V), 0.0), F0, roughness);\n"
"\n"
"    vec3  numerator   = ndf * G * F;\n"
"    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + EPSILON;\n"
"    vec3  specular    = numerator / denominator;\n"
"\n"
"    vec3 Ks = F;\n"
"    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic);\n"
"\n"
"    float NdotL = max(dot(N, L), 0.0);\n"
"\n"
"    float shadow = dirlight_shadow(idx, position);\n"
"\n"
"    return shadow * (Kd * albedo/PI + specular) * light_strength * light_diffuse * NdotL;\n"
"}\n";



const std::string dirlight_contribution =
"vec3 dirlight_contribution( int idx, vec3 position, vec3 F0, vec3 N, vec3 V, vec3 R,\n"
"                            vec3 albedo, float metallic, float roughness, float ao, float emission )\n"
"{\n"
"    DirLight light       = un_dirlights[idx];\n"
"    vec3  light_dir      = light.direction.xyz;\n"
"    vec3  light_ambient  = light.ambient.xyz;\n"
"    vec3  light_diffuse  = light.diffuse.xyz;\n"
"\n"
"    const vec3 L  = normalize(-light_dir);\n"
"    const vec3 H  = normalize(V + L);\n"
"\n"
"    vec3 radiance = light_diffuse;\n"
"\n"
"    float ndf = NDF(roughness, N, H);\n"
"    float G = GSF(roughness, N, V, L);\n"
"    vec3  F = fresnelSchlickR(max(dot(H, V), 0.0), F0, roughness);\n"
"\n"
"    vec3  numerator   = ndf * G * F;\n"
"    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + EPSILON;\n"
"    vec3  specular    = numerator / denominator;\n"
"\n"
"    vec3 Ks = F;\n"
"    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic);\n"
"\n"
"    float NdotL = max(dot(N, L), 0.0);\n"
"\n"
"    return emission*albedo + (Kd * albedo/PI + specular) * radiance * NdotL;\n"
"}\n";


const std::string pointlight_contribution =
"vec3 pointlight_contribution( int idx, vec3 position, vec3 F0, vec3 N, vec3 V, vec3 R,\n"
"                              vec3 albedo, float metallic, float roughness, float ao, float emission )\n"
"{\n"
"    PointLight light = ubo_pointlights[idx];\n"
"    vec3  light_position = light.position.xyz;\n"
"    vec3  light_ambient  = light.ambient.xyz;\n"
"    vec3  light_diffuse  = light.diffuse.xyz;\n"
"    vec3  falloff        = light.attenuation.xyz;\n"
"\n"
"    const vec3 L  = normalize(light_position - position);\n"
"    const vec3 H  = normalize(V + L);\n"
"\n"
"    float d = distance(light_position, position);\n"
"    float attenuation = 1.0 / (falloff.x + d*falloff.y + d*d*falloff.z);\n"
"    vec3  radiance    = light_diffuse * attenuation;\n"
"\n"
"    float ndf = NDF(roughness, N, H);\n"
"    float G = GSF(roughness, N, V, L);\n"
"    vec3  F = fresnelSchlickR(max(dot(H, V), 0.0), F0, roughness);\n"
"\n"
"    vec3  numerator   = ndf * G * F;\n"
"    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + EPSILON;\n"
"    vec3  specular    = numerator / denominator;\n"
"\n"
"    vec3 Ks = F;\n"
"    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic);\n"
"\n"
"    float NdotL = max(dot(N, L), 0.0);\n"
"\n"
"    return emission*albedo + (Kd * albedo / PI + specular) * radiance * NdotL;\n"
"}\n";


};
