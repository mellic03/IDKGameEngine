#define MAX_POINTLIGHTS 10
#define MAX_SPOTLIGHTS 10
#define MAX_DIRLIGHTS 3

struct PointLight
{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 attenuation;
};

struct SpotLight
{
    vec4 position;
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 attenuation;
    vec4 cutoff;
};

struct DirLight
{
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
};

layout (std140, binding = 3) uniform UBO_pointlights
{
    int         ubo_num_pointlights;
    PointLight  ubo_pointlights[MAX_POINTLIGHTS];
};

layout (std140, binding = 4) uniform UBO_spotlights
{
    int         ubo_num_spotlights;
    SpotLight   ubo_spotlights[MAX_SPOTLIGHTS];
};

layout (std140, binding = 5) uniform UBO_dirlights
{
    int         ubo_num_dirlights;
    DirLight    ubo_dirlights[MAX_DIRLIGHTS];
    mat4        ubo_dirlight_lightspacematrices[MAX_DIRLIGHTS];
};
uniform sampler2D   un_dirlight_depthmaps[MAX_DIRLIGHTS];

