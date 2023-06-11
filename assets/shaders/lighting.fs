#version 440 core

#define NUM_POINTLIGHTS 10
#define NUM_SPOTLIGHTS 1

#define NUM_SHADOW_CASCADES 6

struct Material {
  sampler2D diffuseMap, specularMap, emissionMap, normalMap;
  float spec_exponent;
};
uniform Material material;


struct DirLight {
  vec3 position, direction;
  vec3 diffuse;
  vec3 ambient;
  float bias;
  float fog_intensity;
};
uniform DirLight shadowmapped_dirlight;


struct PointLight {
  float radius;
  vec3 ambient, diffuse;
  vec3 position;
  float constant, linear, quadratic;
  float bias;
  float fog_constant, fog_linear, fog_quadratic;
  float fog_intensity;
};
uniform PointLight pointlights[NUM_POINTLIGHTS];
uniform int num_active_pointlights;

struct ShadowPointLight {
  float radius;
  vec3 ambient, diffuse;
  vec3 position;
  float constant, linear, quadratic;
  float bias;
  float fog_constant, fog_linear, fog_quadratic;
  float fog_intensity;
  samplerCube depthCubemap;
};
uniform ShadowPointLight shadow_pointlights[NUM_POINTLIGHTS];
uniform int num_shadow_pointlights;


uniform float shadow_cascades[NUM_SHADOW_CASCADES];
uniform mat4  dir_lightSpaceMatrices[NUM_SHADOW_CASCADES];
uniform sampler2D dir_depthmaps[NUM_SHADOW_CASCADES];


out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gEmission;

uniform mat4 dir_lightSpaceMatrix;
uniform sampler2D depthmap_dirlight;

uniform vec3 viewPos;



vec3 calculate_shadowmapped_pointlight(ShadowPointLight light, vec3 albedo, vec3 fragPos, vec3 normal, float spec_strength, float spec_exponent)
{
    float d = length(light.position - fragPos);
    if (d > light.radius)
        return vec3(0.0);

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), spec_exponent);
    
    float attenuation = 1.0 / (light.constant + d*light.linear + d*d*light.quadratic);
    attenuation *= 1.0 - clamp(d/light.radius, 0.0, 1.0);

    vec3 diffuse  = attenuation * albedo * diff * light.diffuse;
    vec3 ambient  = attenuation * albedo * light.ambient;
    vec3 specular = attenuation * albedo * spec * light.diffuse * spec_strength;

    float shadow = calculate_shadow_pointlight(light, viewPos, fragPos);


    return  (ambient + (1.0 - shadow) * (diffuse + specular));
}


vec3 calculate_pointlight(PointLight light, vec3 albedo, vec3 fragPos, vec3 normal, float spec_strength, float spec_exponent)
{
    float d = length(light.position - fragPos);
    if (d > light.radius)
        return vec3(0.0);

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), spec_exponent);
    
    float attenuation = 1.0 / (light.constant + d*light.linear + d*d*light.quadratic);
    attenuation *= 1.0 - d/light.radius;

    vec3 diffuse  = attenuation * albedo * diff * light.diffuse;
    vec3 ambient  = attenuation * albedo * light.ambient;
    vec3 specular = attenuation * albedo * spec * light.diffuse * spec_strength;

    return  (ambient + diffuse + specular);
}


vec3 calculate_dirlight(DirLight light, vec3 albedo, vec3 fragPos, vec3 normal, float spec_strength, float spec_exponent)
{
    vec3 lightDir = normalize(light.position);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), spec_exponent);


    vec3 ambient  = albedo * light.ambient;
    vec3 diffuse  = albedo * diff * light.diffuse;
    vec3 specular = albedo * spec * light.diffuse * spec_strength;


    vec4 fragPosViewSpace = view * vec4(fragPos.xyz, 1.0);
    float depthValue = abs(fragPosViewSpace.z);
    int cascade = 0;
    for (cascade=0; cascade<NUM_SHADOW_CASCADES; cascade++)
    {
        if (depthValue <= shadow_cascades[cascade])
        break;
    }

    float shadow = calculate_shadow_dirlight(light.position, fragPos, normal, normalize(-light.position), light.bias, cascade);


    return (ambient + (1.0 - shadow) * (diffuse + specular));
}




void main()
{
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 albedo = texture(gAlbedoSpec, TexCoords).rgb;
    vec3 emission = texture(gEmission, TexCoords).rgb;
    float specular_map = texture(gAlbedoSpec, TexCoords).a;
    float spec_exponent = texture(gNormal, TexCoords).a;

    vec3 result = vec3(0.0);

    result += emission;

    for (int i=0; i<num_shadow_pointlights; i++)
    {
        result += calculate_shadowmapped_pointlight(shadow_pointlights[i], albedo, fragPos, normal, specular_map, spec_exponent);
    }

    for (int i=0; i<num_active_pointlights; i++)
    {
        result += calculate_pointlight(pointlights[i], albedo, fragPos, normal, specular_map, spec_exponent);
    }

    result += calculate_dirlight(shadowmapped_dirlight, albedo, fragPos, normal, specular_map, spec_exponent);

  FragColor = vec4(result, 1.0);
}

