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
"#define PI 3.14159265359"
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
"\n"
"vec3 lambert_diffuse( vec3 albedo, vec3 normal, vec3 incoming )\n"
"{\n"
"    return (albedo / 3.14159) * dot(normal, incoming);\n"
"}\n"
"\n"
"float NDF_GGX( vec3 normal, vec3 halfnormal, float roughness )\n"
"{\n"
"    const float alpha = roughness*roughness;\n"
"    const float A = dot(normal, halfnormal);\n"
"    const float B = (A*A) * (alpha*alpha - 1.0) + 1.0;\n"
"    return alpha / ( PI * B*B );\n"
"}\n"
"\n"
"float schlick_GGX( vec3 normal, vec3 halfnormal, vec3 viewdir, float roughness )\n"
"{\n"
"    const float a   = roughness*roughness + 1.0;\n"
"    const float K   = (a*a) / 8.0;\n"
"    const float dnv = dot(normal, viewdir);\n"
"    return dnv / (dnv * (1.0 - K) + K);\n"
"}\n"
"\n"
"vec3 fresnel( vec3 halfnormal, vec3 viewdir, vec3 reflectivity )\n"
"{\n"
"    float dvh = 1.0 - dot(viewdir, halfnormal);\n"
"    dvh = dvh * dvh * dvh * dvh * dvh;\n"
"    return reflectivity + (1.0 - reflectivity) * dvh;\n"
"}\n"
"\n"
"vec3 specular_BDRF( vec3 normal, vec3 halfnormal, vec3 lightdir, vec3 viewdir, float roughness )\n"
"{\n"
"    const vec3 reflectivity = vec3(1.0, 0.86, 0.57);\n"
"    const float D = NDF_GGX(normal, halfnormal, roughness);\n"
"    const float G = schlick_GGX(normal, halfnormal, viewdir, roughness);\n"
"    const vec3  F = fresnel(halfnormal, viewdir, reflectivity);\n"
"    return (D * G * F) / (dot(normal, lightdir) * dot(normal, viewdir));\n"
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
"    vec2  roughness_ao = texture( un_texture_3, fsin_texcoords).rg;\n"
"    float roughness    = roughness_ao.r;\n"
"    float ao           = roughness_ao.g;\n"
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
", view_dir, position, normal, albedo, metallic, roughness, ao);\n";


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
"    vec3 ambient  = albedo * diffuse_f * light_ambient;\n"
"    vec3 diffuse  = albedo * diffuse_ff * light_diffuse;\n"
"    vec3 specular = vec3(0.0); // diffuse * specular_f * 15*spec;\n"
"\n"
"    return ambient + diffuse + specular;\n"
"}\n";



const std::string pointlight_contribution =
"vec3 pointlight_contribution( int idx, vec3 view_dir, vec3 position, vec3 normal,\n"
"                              vec3 albedo, float metallic, float roughness, float ao )\n"
"{\n"
"    vec3 light_position = ubo_pointlights[idx].position.xyz;\n"
"    vec3 light_ambient = ubo_pointlights[idx].ambient.xyz;\n"
"    vec3 light_diffuse = ubo_pointlights[idx].diffuse.xyz;\n"
"\n"
"    float attenuation_constant = ubo_pointlights[idx].attenuation.x;\n"
"    float attentuation_linear = ubo_pointlights[idx].attenuation.y;\n"
"    float attentuation_quadratic = ubo_pointlights[idx].attenuation.z;\n"
"\n"
"    float d = distance(position, light_position);\n"
"    float attenuation = max(1.0 / (d*d), 0.00001);\n"
"\n"
"    vec3 frag_to_light = normalize(light_position - position);\n"
"    float diffuse_f = max(dot(normal, frag_to_light), 0.0);\n"
"    vec3 halfway_dir = normalize(frag_to_light + view_dir);  \n"
"\n"
"    const vec3 reflectivity = vec3(1.0, 0.86, 0.57);\n"
"    vec3 kS = fresnel(halfway_dir, view_dir, reflectivity);\n"
"    vec3 kD = 1.0 - kS;\n"
"\n"
"    vec3 ambient  = attenuation * albedo * light_ambient;\n"
"    vec3 diffuse  = attenuation * albedo * kD * kS / PI;\n"
"    vec3 specular = attenuation * specular_BDRF(normal, halfway_dir, frag_to_light, view_dir, roughness);\n"
"\n"
"    return ambient + light_diffuse * (diffuse + specular) * dot(normal, frag_to_light);\n"
"}\n";


};
