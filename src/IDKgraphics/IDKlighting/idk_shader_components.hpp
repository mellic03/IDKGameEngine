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
"\n"
"in vec2 fsin_texcoords;\n"
"\n"
"uniform sampler2D un_texture_0;\n"
"uniform sampler2D un_texture_1;\n"
"uniform sampler2D un_texture_2;\n"
"uniform sampler2D un_texture_3;\n"
"\n"
"layout (std140, binding = 2) uniform UBO_camera_data\n"
"{\n"
"    mat4 un_view;\n"
"    mat4 un_projection;\n"
"    vec3 un_viewpos;\n"
"};\n"
"\n";


const std::string shader_end_header =
"vec3 fresnel_schlick( vec3 F0, vec3 V, vec3 H )\n"
"{\n"
"    return F0 + (1.0 - F0) * pow(1.0 - max(dot(V, H), 0.0), 5.0);\n"
"}\n"
"\n"
"float NDF( float roughness, vec3 N, vec3 H )\n"
"{\n"
"    float a   = roughness*roughness;\n"
"    float ndh = max(dot(N, H), 0.0);\n"
"    float b   = (ndh*ndh) * (a*a - 1.0) + 1.0;\n"
"    return (a*a) / max(PI * b*b, EPSILON);\n"
"}\n"
"\n"
"float GSF1( float roughness, vec3 normal, vec3 X )\n"
"{\n"
"    float k     = (roughness*roughness) / 2.0;\n"
"    float num   = max(dot(normal, X), 0.0);\n"
"    float denom = dot(normal, X) * (1.0 - k) + k;\n"
"    denom       = max(denom, EPSILON);\n"
"    return num / denom;\n"
"}\n"
"float GSF( float roughness, vec3 normal, vec3 V, vec3 L )\n"
"{\n"
"    return GSF1(roughness, normal, V) * GSF1(roughness, normal, L);\n"
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
"void main()\n"
"{\n"
"    vec4  albedo_metallic = texture( un_texture_0, fsin_texcoords );\n"
"    vec3  albedo       = albedo_metallic.rgb;\n"
"    float metallic     = albedo_metallic.a;\n"
"\n"
"    vec3  position     = texture( un_texture_1, fsin_texcoords ).xyz;\n"
"    vec3  normal       = texture( un_texture_2, fsin_texcoords ).xyz;\n"
"\n"
"    vec3  roughness_ao_em = texture( un_texture_3, fsin_texcoords).rgb;\n"
"    float roughness    = roughness_ao_em.r;\n"
"    float ao           = roughness_ao_em.g;\n"
"    float emission     = roughness_ao_em.b;\n"
"\n"
"    vec3  view_dir     = normalize(un_viewpos - position);\n"
"    vec3  color        = vec3(0.0);"
"\n\n";


const std::string shader_end_main =
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
"    color += pointlight_contribution(";

const std::string pointlight_shadowmapped_call_begin =
"    color += pointlight_contribution_shadowmapped(";

const std::string dirlight_call_begin =
"    color += dirlight_contribution(";

const std::string dirlight_shadowmapped_call_begin =
"    color += dirlight_contribution_shadowmapped(";

const std::string light_call_end =
", view_dir, position, normal, albedo, metallic, roughness, ao, emission);\n";


const std::string dirlight_contribution_shadowmapped =
"float dirlight_shadow(int idx, vec3 position)\n"
"{\n"
"    vec4 fragpos_lightspace = un_dirlight_matrices[idx] * vec4(position, 1.0);\n"
"    vec3 projCoords = fragpos_lightspace.xyz / fragpos_lightspace.w;\n"
"\n"
"    projCoords = projCoords * 0.5 + 0.5; \n"
"    float bias = 0.001;\n"
"    float shadow = 0.0;\n"
"    vec2 texelSize = 1.0 / textureSize(un_dirlight_depthmaps[idx], 0);\n"
"\n"
"    for(int x = -1; x <= 1; ++x)\n"
"    {\n"
"        for(int y = -1; y <= 1; ++y)\n"
"        {\n"
"            float pcfDepth = texture(un_dirlight_depthmaps[idx], projCoords.xy + vec2(x, y) * texelSize).r; \n"
"            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;        \n"
"        }    \n"
"    }\n"
"\n"
"    shadow /= 9.0;\n"
"    return 1.0 - shadow;\n"
"}\n"
"\n"
"vec3 dirlight_contribution_shadowmapped( int idx, vec3 view_dir, vec3 position, vec3 normal,\n"
"                            vec3 albedo, float metallic, float roughness, float ao )\n"
"{\n"
"    DirLight light = un_dirlights[idx];\n"
"    vec3 light_direction = normalize(light.direction.xyz);\n"
"    vec3 light_ambient = light.ambient.xyz;\n"
"    vec3 light_diffuse = light.diffuse.xyz;\n"
"\n"
"    vec3 frag_to_light = normalize(-light_direction);\n"
"    float diffuse_ff = max(dot(normal, frag_to_light), 0.0);\n"
"    float diffuse_f = dot(normal, frag_to_light);\n"
"    diffuse_f = (diffuse_f + 1.0) / 2.0;\n"
"\n"
"    vec3 halfway_dir = normalize(frag_to_light + view_dir);\n"
"    float specular_f = 0.0; // pow(max(dot(normal, halfway_dir), 0.0), spec_exponent);\n"
"\n"
"    float shadow = dirlight_shadow(idx, position);\n"
"    vec3 ambient  = albedo * diffuse_f * light_ambient;\n"
"    vec3 diffuse  = albedo * diffuse_ff * light_diffuse;\n"
"    vec3 specular = vec3(0.0); // diffuse * specular_f * 15*spec;\n"
"\n"
"    return ambient + shadow * (diffuse + specular);\n"
"}\n";



const std::string dirlight_contribution =
"vec3 dirlight_contribution( int idx, vec3 view_dir, vec3 position, vec3 normal,\n"
"                            vec3 albedo, float metallic, float roughness, float ao, float emission )\n"
"{\n"
"    DirLight light = un_dirlights[idx];\n"
"    vec3 light_direction = normalize(light.direction.xyz);\n"
"    vec3 light_ambient = light.ambient.xyz;\n"
"    vec3 light_diffuse = light.diffuse.xyz;\n"
"\n"
"    vec3 frag_to_light = normalize(-light_direction);\n"
"    float diffuse_ff = max(dot(normal, frag_to_light), 0.0);\n"
"    float diffuse_f = dot(normal, frag_to_light);\n"
"    diffuse_f = (diffuse_f + 1.0) / 2.0;\n"
"\n"
"    vec3 halfway_dir = normalize(frag_to_light + view_dir);\n"
"    float specular_f = 0.0; // pow(max(dot(normal, halfway_dir), 0.0), spec_exponent);\n"
"\n"
"    const vec3 reflectivity = vec3(1.0, 0.86, 0.57);\n"
"    vec3 kS = fresnel(halfway_dir, view_dir, reflectivity);\n"
"    vec3 kD = 1.0 - kS;\n"
"\n"
"    vec3 ambient  = albedo * light_ambient;\n"
"    vec3 diffuse  = albedo * kD * kS / PI;\n"
"    vec3 specular = specular_BDRF(normal, halfway_dir, frag_to_light, view_dir, roughness);\n"
"\n"
"    return ambient + light_diffuse * (diffuse + specular) * max(dot(normal, frag_to_light), 0.0);\n"
"}\n";



const std::string pointlight_contribution =
"vec3 pointlight_contribution( int idx, vec3 view_dir, vec3 position, vec3 normal,\n"
"                              vec3 albedo, float metallic, float roughness, float ao, float emission )\n"
"{\n"
"    PointLight light = ubo_pointlights[idx];\n"
"    vec3  light_position = light.position.xyz;\n"
"    vec3  light_ambient  = light.ambient.xyz;\n"
"    vec3  light_diffuse  = light.diffuse.xyz;\n"
"    vec3  falloff        = light.attenuation.xyz;\n"
"\n"
"    const vec3 F0 = vec3(0.56, 0.57, 0.58);\n"
"    const vec3 N  = normalize(normal);\n"
"    const vec3 L  = normalize(light_position - position);\n"
"    const vec3 V  = normalize(view_dir);\n"
"    const vec3 H  = normalize(V + L);\n"
"\n"
"    vec3 Ks = fresnel_schlick(F0, V, H) * metallic;\n"
"    vec3 Kd = (vec3(1.0) - Ks);\n"
"\n"
"    vec3 cookTorrance = cook_torrance(min(roughness+0.01, 1.0), N, V, L, H, Ks);\n"
"    vec3 BRDF = Kd * (albedo / PI) + cookTorrance;\n"
"\n"
"    float d = distance(light_position, position);\n"
"    float attenuation = 1.0 / (falloff.x + d*falloff.y + d*d*falloff.z);\n"
"\n"
"    return emission*albedo + attenuation*light_ambient + attenuation * light_diffuse * BRDF * max(dot(L, N), 0.0);\n"
"}\n";


};
