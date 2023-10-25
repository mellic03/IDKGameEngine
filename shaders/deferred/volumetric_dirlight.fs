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

#define MAX_STEPS 16

const float c_goldenRatioConjugate = 0.61803398875f;

float hash13(vec3 p3)
{
	p3  = fract(p3 * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

void main()
{
    vec3  fragpos = texture(un_texture_1, fsin_texcoords).xyz;
    float fragdist = length(fragpos - un_viewpos);    
    vec3  ray_dir = normalize(fragpos - un_viewpos);
    float step_size = fragdist / MAX_STEPS;


    float ray_offset = texture(un_whitenoise, vec3(fsin_texcoords, un_increment_2)).r;
    ray_offset = (ray_offset + 1.0) / 2.0;
    ray_offset *= 4.0*step_size;

    vec3 ray_pos = un_viewpos + ray_offset*ray_dir;
    float ray_dist = 0.0;
    vec3 accum = vec3(0.0);

    vec3 offset = vec3(-50*un_increment_0, 0.0, 50*un_increment_0);

    for (float i=0; i<MAX_STEPS; i++)
    {
        // float displacement = 0.2*(texture(un_worley, ray_pos + 0.01*offset).r - 0.5);
        float density      = texture(un_worley, ray_pos + 0.01*offset).r;
        float intensity    = 0.4;


        // float shadow = dirlight_shadow(idx, ray_pos+displacement);
        float shadow = dirlight_shadow(0, ray_pos);
        vec3 diffuse = ubo_dirlights[0].diffuse.xyz;
        accum += intensity * density * shadow * diffuse;

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

