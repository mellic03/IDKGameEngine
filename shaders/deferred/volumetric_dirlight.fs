#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;

uniform sampler2D un_texture_0;
uniform sampler2D un_texture_1;
uniform sampler2D un_texture_2;
uniform sampler2D un_texture_3;

uniform sampler3D un_worley;
uniform float un_increment_0;
uniform float un_increment_1;
uniform float un_increment_2;

layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
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

#define MAX_STEPS 256


void main()
{
    vec3  fragpos = texture(un_texture_1, fsin_texcoords).xyz;
    float fragdist = length(fragpos - un_viewpos);    
    vec3  ray_dir = normalize(fragpos - un_viewpos);
    float step_size = 0.15;

    vec3 diffff = ubo_dirlights[0].diffuse.xyz;

    if (fragdist > MAX_STEPS*step_size)
    {
        vec3 dirlight_dir = normalize(ubo_dirlights[0].direction.xyz);
        float mie = miePhase(-dot(ray_dir, dirlight_dir));

        fsout_frag_color = vec4(15.0*mie*diffff, 1.0);
        return;
    }


    vec3 ray_pos = un_viewpos;
    float ray_dist = 0.0;
    vec3 accum = vec3(0.0);

    vec3 offset = vec3(-50*un_increment_0, 0.0, 50*un_increment_0);

    for (float i=0; i<MAX_STEPS; i++)
    {
        float displacement = 0.2*(texture(un_worley, ray_pos + 0.01*offset).r - 0.5);
        float density      = texture(un_worley, ray_pos + 0.01*offset).r;
        float intensity    = 0.02;


        for (int idx = 0; idx < ubo_num_dirlights; idx++)
        {
            float shadow = dirlight_shadow(idx, ray_pos+displacement);
            vec3 diffuse = ubo_dirlights[idx].diffuse.xyz;
            accum += intensity * density * shadow * diffuse;
        }

        if (ray_dist >= fragdist)
        {
            break;
        }

        ray_pos += step_size * ray_dir;
        ray_dist += step_size;
    }

    vec3 dirlight_dir = normalize(ubo_dirlights[0].direction.xyz);
    // float angle = dot(ray_dir, dirlight_dir);
    // float rayleigh = ((3.0*3.14159) / 16.0) * (1.0 + angle * angle);
    float mie = miePhase(-dot(ray_dir, dirlight_dir));

    fsout_frag_color = vec4(mie*accum, 1.0);
    // fsout_frag_color = vec4(0.0, 0.0, 0.0, 1.0);
}

