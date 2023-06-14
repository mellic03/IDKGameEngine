#version 440 core

#include "structs.glh"

#define SQ(a) ((a)*(a))

layout (location = 0) out vec4 FragColor1;
layout (location = 1) out vec4 FragColor2;
layout (location = 2) out vec4 FragColor3;
layout (location = 3) out vec4 FragColor4;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec2 fsin_texcoords;

uniform vec3 un_viewpos;
uniform pointlight un_pointlights[10];
uniform int un_num_pointlights;

uniform sampler2D un_albedo_texture;
uniform sampler2D un_specular_texture;


vec3 pointlight_contribution(int idx, vec3 view_dir, vec3 albedomap, vec3 specularmap)
{
    float spec_exponent = 32.0;

    pointlight light = un_pointlights[idx];
    float d = distance(fsin_fragpos, light.position);

    vec3 frag_to_light = normalize(light.position - fsin_fragpos);
    float diffuse_f = max(dot(fsin_normal, frag_to_light), 0.0);

    vec3 halfway_dir = normalize(frag_to_light + view_dir);  
    float specular_f = pow(max(dot(fsin_normal, halfway_dir), 0.0), spec_exponent);

    float attenuation = 1.0 / (
          light.attenuation_constant
        + light.attentuation_linear * d
        + light.attentuation_quadratic * d*d
    );

    vec3 ambient  = attenuation * albedomap * light.ambient;
    vec3 diffuse  = attenuation * albedomap * diffuse_f * light.diffuse;
    vec3 specular = attenuation * albedomap * specular_f * specularmap;

    vec3 result = ambient + diffuse + specular;

    return result;
}


void main()
{
    vec3 view_dir = normalize(-un_viewpos - fsin_fragpos);
    vec3 albedo_map = texture(un_albedo_texture, fsin_texcoords).rgb;
    vec3 specular_map = texture(un_specular_texture, fsin_texcoords).rgb;

    vec3 color = vec3(0.0);
    for (int i = 0; i < un_num_pointlights; i++)
    {
        color += pointlight_contribution(i, view_dir, albedo_map, specular_map);
    }

    FragColor1 = vec4(color, 1.0);
    FragColor2 = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor3 = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor4 = vec4(0.0, 0.0, 0.0, 1.0);
}
