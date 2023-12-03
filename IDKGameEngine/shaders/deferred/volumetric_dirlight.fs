#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;

uniform sampler2D un_texture_0;
uniform sampler2D un_texture_1;
uniform sampler2D un_texture_2;
uniform sampler2D un_texture_3;

uniform sampler3D un_worley;
uniform sampler3D un_whitenoise;
uniform float un_increment_0;
uniform float un_increment_1;
uniform float un_increment_2;

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

#include "deferred/UBO_lightsources.glsl"
#include "deferred/methods.glsl"


float PHG (float g, float cosTheta)
{
    const float Inv4Pi = 0.07957747154594766788;
    
    float gSq = g * g;
    float denomPreMul = 1 + gSq - (2.0 * g * cosTheta);
    return (1 - gSq) * Inv4Pi * inversesqrt(denomPreMul * denomPreMul * denomPreMul);
}


float miePhase (float cosTheta)
{
    return mix (PHG (0.8, cosTheta), PHG (-0.5, cosTheta), 0.5);
}



#define MAX_STEPS 16
#define INTENSITY 0.25
#define DENSITY_BIGNESS 0.35

void main()
{
    // vec3  frag_pos = texture(un_texture_1, fsin_texcoords).xyz;
    // float frag_dist = distance(un_viewpos, frag_pos) - 0.01;
    // float step_size = frag_dist / MAX_STEPS;
    // const float intensity = INTENSITY*step_size;

    // float ray_offset = 4.0 * step_size * texture(un_whitenoise, vec3(16.0*fsin_texcoords, un_increment_2)).r;

    // vec3 ray_dir = normalize(frag_pos - un_viewpos);
    // vec3 ray_pos = un_viewpos + ray_offset*ray_dir;

    // step_size = distance(ray_pos, frag_pos) / MAX_STEPS;

    // vec3 accum  = vec3(0.0);
    // vec3 offset = vec3(-un_increment_0, 0.0, un_increment_0);

    // for (float i=0; i<MAX_STEPS; i++)
    // {
    //     float density = texture(un_worley, DENSITY_BIGNESS * (ray_pos + offset)).r;

    //     float shadow = dirlight_shadow(0, ray_pos);
    //     vec3 diffuse = ubo_dirlights[0].diffuse.xyz;
    //     accum += density * shadow * diffuse;

    //     ray_pos += step_size * ray_dir;
    // }

    // vec3 dirlight_dir = normalize(ubo_dirlights[0].direction.xyz);
    // float mie = miePhase(-dot(ray_dir, dirlight_dir));
    // accum *= mie * INTENSITY * step_size;

    // fsout_frag_color = vec4(accum, 1.0);

    fsout_frag_color = vec4(0.0);
}

