#version 440 core

#define MAX_POINTLIGHTS 10


layout (location = 0) out vec4 fsout_albedo_metallic;
layout (location = 1) out vec4 fsout_position;
layout (location = 2) out vec4 fsout_normal;
layout (location = 3) out vec4 fsout_roughness_ao;

in vec3 fsin_fragpos;
in vec3 fsin_normal;
in vec2 fsin_texcoords;

uniform sampler2D un_albedo_texture;
uniform sampler2D un_metallic_texture;
uniform sampler2D un_roughness_texture;
uniform sampler2D un_ao_texture;


layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};


void main()
{
    vec3  albedo    = texture(un_albedo_texture, fsin_texcoords).rgb;
    float metallic  = texture(un_metallic_texture, fsin_texcoords).r;
    float roughness = texture(un_roughness_texture, fsin_texcoords).r;
    float ao        = texture(un_ao_texture, fsin_texcoords).r;

    fsout_albedo_metallic = vec4(albedo, metallic);
    fsout_position        = vec4(fsin_fragpos, 1.0);
    fsout_normal          = vec4(normalize(fsin_normal), 1.0);
    fsout_roughness_ao    = vec4(roughness, ao, 0.0, 1.0);
}
