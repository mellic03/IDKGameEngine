#version 440 core

layout (location = 0) out vec4 fsout_frag_color;


#define MAX_POINTLIGHTS 10
#define MAX_SPOTLIGHTS  10
#define MAX_DIRLIGHTS   10

#define NUM_POINTLIGHTS 0
#define NUM_SPOTLIGHTS  0
#define NUM_DIRLIGHTS   0


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


struct DirLight
{
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
};



layout (std140, binding = 5) uniform UBO_dirlights
{
    DirLight    un_dirlights[MAX_DIRLIGHTS];
    mat4        un_dirlight_matrices[MAX_DIRLIGHTS];
};

uniform sampler2D   un_dirlight_depthmaps[MAX_DIRLIGHTS];



float dirlight_shadow(int idx, vec3 position)
{
    vec4 fragpos_lightspace = un_dirlight_matrices[idx] * vec4(position, 1.0);

    vec3 projCoords = fragpos_lightspace.xyz / fragpos_lightspace.w;
    projCoords = projCoords * 0.5 + 0.5; 

    float bias = 0.001;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(un_dirlight_depthmaps[idx], 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(un_dirlight_depthmaps[idx], projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return 1.0 - shadow;
}


vec3 dirlight_contribution( int idx, vec3 view_dir, vec3 position,
                            vec3 normal, vec3 albedo, float spec,
                            float spec_exponent )
{
    DirLight light = un_dirlights[idx];

    vec3 light_direction = normalize(light.direction.xyz);
    vec3 light_ambient = light.ambient.xyz;
    vec3 light_diffuse = light.diffuse.xyz;

    vec3 frag_to_light = normalize(-light_direction);
    float diffuse_ff = max(dot(normal, frag_to_light), 0.0);
    float diffuse_f = dot(normal, frag_to_light);
    diffuse_f = (diffuse_f + 1.0) / 2.0;

    vec3 halfway_dir = normalize(frag_to_light + view_dir);
    float specular_f = pow(max(dot(normal, halfway_dir), 0.0), spec_exponent);

    float shadow = dirlight_shadow(idx, position);

    vec3 ambient  = albedo * diffuse_f * light_ambient;
    vec3 diffuse  = albedo * diffuse_ff * light_diffuse;
    vec3 specular = diffuse * specular_f * 255*spec;

    vec3 result = ambient + shadow*diffuse + shadow*specular;

    return result; 
}



void main()
{
    vec4  albedospec = texture( un_texture_0, fsin_texcoords );
    vec3  albedo     = albedospec.rgb;
    float specular   = albedospec.a;

    vec3  position   = texture( un_texture_1, fsin_texcoords ).xyz;
    vec3  normal     = texture( un_texture_2, fsin_texcoords ).xyz;
    float spec_exp   = texture( un_texture_2, fsin_texcoords).a;

    vec3  emission   = texture( un_texture_3, fsin_texcoords ).xyz;
    vec3  view_dir   = normalize(un_viewpos - position);

    vec3 color = vec3(0.0);


    for (int i=0; i<NUM_DIRLIGHTS; i++)
    {
        color += dirlight_contribution(
            i,       view_dir,   position,
            normal,  albedo,     specular,
            spec_exp
        );
    }


    // fsout_frag_color.rgb = texture(un_dirlight_depthmaps[2], fsin_texcoords).rgb;
    fsout_frag_color = vec4(color, 1.0);
}
