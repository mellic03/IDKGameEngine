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
