struct pointlight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;

    float attenuation_constant;
    float attentuation_linear;
    float attentuation_quadratic;

};

struct dirlight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
};

struct spotlight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;

    float attenuation_constant;
    float attentuation_linear;
    float attentuation_quadratic;

    float           inner_cutoff;
    float           outer_cutoff;
};


#define MAX_POINTLIGHTS 10
#define MAX_SPOTLIGHTS 10

layout (std140, binding = 3) uniform UBO_pointlights
{                                                               // offset           size
    int         ubo_num_pointlights;                            // 0                4
    vec4        ubo_pointlight_position[MAX_POINTLIGHTS];       // 16               160
    vec4        ubo_pointlight_ambient[MAX_POINTLIGHTS];        // 176              160
    vec4        ubo_pointlight_diffuse[MAX_POINTLIGHTS];        // 336              160
    vec4        ubo_pointlight_attenuation[MAX_POINTLIGHTS];    // 496              160
};

layout (std140, binding = 4) uniform UBO_spotlights
{                                                              // offset           size
    int         ubo_num_spotlights;                            // 0                4
    vec4        ubo_spotlight_position[MAX_SPOTLIGHTS];        // 16               160
    vec4        ubo_spotlight_direction[MAX_SPOTLIGHTS];       // 176              160
    vec4        ubo_spotlight_ambient[MAX_SPOTLIGHTS];         // 336              160
    vec4        ubo_spotlight_diffuse[MAX_SPOTLIGHTS];         // 496              160
    vec4        ubo_spotlight_attenuation[MAX_SPOTLIGHTS];     // 656              160
    vec4        ubo_spotlight_cutoff[MAX_SPOTLIGHTS];          // 816              160
};
