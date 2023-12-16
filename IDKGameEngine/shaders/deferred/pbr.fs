// #version 440 core

// layout (location = 0) out vec4 fsout_frag_color;


// #define MAX_POINTLIGHTS 10
// #define MAX_SPOTLIGHTS  10
// #define MAX_DIRLIGHTS   10

// #define NUM_POINTLIGHTS 0
// #define NUM_SPOTLIGHTS  0
// #define NUM_DIRLIGHTS   0

// #define PI 3.14159265359
// #define EPSILON 0.00001


// in vec2 fsin_texcoords;

// uniform sampler2D un_texture_0; // albedo_metallic
// uniform sampler2D un_texture_1; // position
// uniform sampler2D un_texture_2; // normal
// uniform sampler2D un_texture_3; // roughness



// struct Camera
// {
//     vec4 position;
//     vec4 beg;
//     vec4 aberration_rg;
//     vec4 aberration_b;
//     vec4 exposure;
// };

// layout (std140, binding = 2) uniform UBO_camera_data
// {
//     mat4 un_view;
//     mat4 un_projection;
//     vec3 un_viewpos;
//     Camera un_camera;
// };

// struct DirLight
// {
//     vec4 direction;
//     vec4 ambient;
//     vec4 diffuse;
// };
// layout (std140, binding = 5) uniform UBO_dirlights
// {
//     DirLight    un_dirlights[MAX_DIRLIGHTS];
//     mat4        un_dirlight_matrices[MAX_DIRLIGHTS];
// };
// uniform sampler2D   un_dirlight_depthmaps[MAX_DIRLIGHTS];


// struct PointLight
// {
//     vec4 position;
//     vec4 ambient;
//     vec4 diffuse;
//     vec4 attenuation;
// };
// layout (std140, binding = 3) uniform UBO_pointlights
// {
//     PointLight  ubo_pointlights[MAX_POINTLIGHTS];
// };



// vec3 fresnel_schlick( vec3 F0, vec3 viewdir, vec3 halfwaydir )
// {
//     return F0 + (1.0 - F0) * pow(1.0 - max(dot(viewdir, halfwaydir), 0.0), 5.0);
// }



// float NDF( float roughness, vec3 normal, vec3 halfwaydir )
// {
//     float a   = roughness*roughness;
//     float ndh = max(dot(normal, halfwaydir), 0.0);
//     float b   = (ndh*ndh) * (a*a - 1.0) + 1.0;
    
//     return (a*a) / (PI * b*b);
// }


// float GSF( float roughness, vec3 normal, vec3 viewdir )
// {
//     float k   = (roughness*roughness) / 2.0;
//     float ndv = max(dot(normal, viewdir), 0.0);
//     return ndv / (ndv * (1.0 - k) + k);
// }


// vec3 cook_torrance( float roughness, vec3 normal, vec3 viewdir, vec3 lightdir, vec3 halfwaydir, vec3 fresnel )
// {
//     float ndf = NDF(roughness, normal, halfwaydir);
//     float gsf = GSF(roughness, normal, viewdir);

//     return (ndf * gsf * fresnel) / (4.0 * max(dot(viewdir, normal), 0.0) * max(dot(lightdir, normal), 0.0));
// }



// vec3 pointlight_contribution( int idx, vec3 view_dir, vec3 position, vec3 normal,
//                               vec3 albedo, float metallic, float roughness, float ao )
// {
//     PointLight light = ubo_pointlights[idx];

//     vec3 light_position = ubo_pointlights[idx].position.xyz;
//     vec3 light_ambient  = ubo_pointlights[idx].ambient.xyz;
//     vec3 light_diffuse  = ubo_pointlights[idx].diffuse.xyz;

//     vec3 frag_to_light = normalize(light_position - position);
//     vec3 halfway_dir = normalize(frag_to_light + view_dir);

//     float Kd = max(dot(frag_to_light, normal), 0.0);
//     float Ks = 1.0 - Kd;

//     const vec3 F0 = vec3(1.0, 0.86, 0.57);
//     vec3 Fs = fresnel_schlick(F0, view_dir, halfway_dir);

//     vec3 diffuse  = albedo / PI;
//     vec3 specular = cook_torrance(roughness, normal, view_dir, frag_to_light, halfway_dir, Fs);

//     return Kd*Fs + Ks*specular;
// }



// void main()
// {
//     vec4  albedo_metallic = texture( un_texture_0, fsin_texcoords );
//     vec3  albedo      = albedo_metallic.rgb;
//     float metallic    = albedo_metallic.a;

//     vec3  position    = texture( un_texture_1, fsin_texcoords ).xyz;
//     vec3  normal      = texture( un_texture_2, fsin_texcoords ).xyz;

//     vec2 roughness_ao = texture( un_texture_3, fsin_texcoords).rg;
//     float roughness   = roughness_ao.r;
//     float ao          = roughness_ao.g;

//     vec3  view_dir   = normalize(un_viewpos - position);

//     vec3 color = vec3(0.0);


    


//     fsout_frag_color = vec4(color, 1.0);
// }

