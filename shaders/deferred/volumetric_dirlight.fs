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

#include "UBO_lightsources.glsl"
#include "methods.glsl"


#define MAX_STEPS 128
#define STEP_SIZE 0.1

void main()
{
    vec3  fragpos = texture( un_texture_1, fsin_texcoords ).xyz;
    float fragdist = length(fragpos - un_viewpos);    
    vec3  ray_dir = normalize(fragpos - un_viewpos);

    float step_size = 0.1;

    vec3 ray_pos = un_viewpos;
    float ray_dist = 0.0;
    vec3 accum = vec3(0.0);


    for (float i=0; i<MAX_STEPS; i++)
    {
        for (int idx = 0; idx < ubo_num_dirlights; idx++)
        {
            vec3 dirlight_dir = normalize(ubo_dirlights[0].direction.xyz);
            float angle = dot(ray_dir, dirlight_dir);
            float rayleigh = ((3.0*3.14159) / 16.0) * (1.0 + angle * angle);

            vec3 offset = vec3(un_increment_1*-85, 0.0, un_increment_0*85);
            float noise = texture(un_worley, 0.3*ray_pos + 0.01*offset).r;

            vec3 offsetw = vec3(-0.1*un_increment_0, 0.0, 0.1*un_increment_0);
            float noisew = 0.2*texture(un_worley, 0.3*ray_pos + 0.01*offsetw).r;

            accum += rayleigh * 0.01 * noise * dirlight_shadow(idx, ray_pos+noisew) * ubo_dirlights[idx].diffuse.xyz;
            accum += 0.002 * noise * ubo_dirlights[0].ambient.xyz;
        }

        if (ray_dist >= fragdist)
        {
            break;
        }

        step_size *= 1.001;
        ray_pos += step_size * ray_dir;
        ray_dist += step_size;
    }

    fsout_frag_color = vec4(accum, 1.0);
}

