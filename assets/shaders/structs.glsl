struct pointlight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 position;

    float attenuation_constant;
    float attentuation_linear;
    float attentuation_quadratic;
};

struct dirlight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 direction;
};

struct spotlight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 position;
    vec3 direction;

    float attenuation_constant;
    float attentuation_linear;
    float attentuation_quadratic;
};
