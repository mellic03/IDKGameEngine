#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;

uniform sampler2D un_texture_0;
uniform sampler2D un_texture_1;
uniform sampler2D un_texture_2;
uniform sampler2D un_texture_3;

layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};

#include "UBO_lightsources.glsl"
#include "methods.glsl"


#define MAX_STEPS 128
#define STEP_SIZE 0.1

void main()
{
    vec3  fragpos = texture( un_texture_1, fsin_texcoords ).xyz;
    float fragdist = length(fragpos - un_viewpos);    
    vec3  dir = normalize(fragpos - un_viewpos);

    float step_size = 0.1;

    vec3 ray_pos = un_viewpos;
    float ray_dist = 0.0;
    vec3 accum = vec3(0.0);

    for (float i=0; i<MAX_STEPS; i++)
    {
        for (int idx = 0; idx < ubo_num_dirlights; idx++)
        {
            accum += 0.1 * dirlight_shadow(idx, ray_pos) * ubo_dirlights[idx].diffuse.xyz;
        }

        if (ray_dist >= fragdist)
        {
            break;
        }

        step_size *= 1.001;
        ray_pos += step_size * dir;
        ray_dist += step_size;
    }

    fsout_frag_color = vec4(accum, 1.0);
}

