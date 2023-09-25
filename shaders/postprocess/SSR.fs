#version 440 core

out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;
uniform sampler2D un_texture_1;
uniform sampler2D un_texture_2;
uniform sampler2D un_texture_3;

uniform sampler2D un_texture_4; // lightingpass result

layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};

uniform vec3 un_cam_dir;

#define MAX_STEPS 128
#define STEP_SIZE 0.1
#define BIAS 0.02


void main()
{
    vec4  albedospec = texture(un_texture_0, fsin_texcoords);
    vec3  albedo     = albedospec.rgb;
    float specular   = albedospec.a;
    vec3  position   = texture(un_texture_1, fsin_texcoords).xyz;
    vec3  normal     = texture(un_texture_2, fsin_texcoords).xyz;
    float  reflection = texture(un_texture_3, fsin_texcoords).r;

    if (reflection < 0.1)
    {
        return;
    }

    vec3 ray_pos = position + BIAS*normal;
    vec3 cam_to_frag = normalize(ray_pos - un_viewpos);
    vec3 ray_dir = normalize(reflect(cam_to_frag, normal));

    float str = 1.0 - clamp(dot(-cam_to_frag, normal), 0.0, 1.0);
    str = smoothstep(0.4, 1, str);


    vec3 result = vec3(0.0);
    int count = 0;
    float cumdist = 0.0;

    for (float i=0; i<MAX_STEPS; i++)
    {
        // Project ray into UV space
        // ---------------------------------------------------------------------------
        vec4 projected = un_projection * un_view * vec4(ray_pos, 1.0);
        projected.xy /= projected.w;
        projected.xy = projected.xy * 0.5 + 0.5;
    
        float frag_depth = (un_projection * un_view * texture(un_texture_1, projected.xy)).z;
        float ray_depth = projected.z;
        // ---------------------------------------------------------------------------

        if (projected.x < 0.0 || projected.x > 1.0 || projected.y < 0.0 || projected.y > 1.0)
        {
            break;
        }

        if (ray_depth >= frag_depth)
        {
            result = texture(un_texture_4, projected.xy).rgb;
            break;
        }

        ray_pos += STEP_SIZE * ray_dir;
        cumdist += STEP_SIZE;
    }

    const float CONSTANT  = 2.0;
    const float LINEAR    = 2.0;
    const float QUADRATIC = 1.0;

    float attenuation = 1.0 / (CONSTANT + LINEAR*cumdist + QUADRATIC*cumdist*cumdist);
    result = str * attenuation * result;

    fsout_frag_color = vec4(result, 1.0);
}
