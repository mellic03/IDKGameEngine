
vec3 pointlight_contribution( int idx, vec3 view_dir, vec3 position,
                              vec3 normal, vec3 albedo, float spec,
                              float spec_exponent )
{
    vec3 light_position = ubo_pointlights[idx].position.xyz;

    vec3 light_ambient = ubo_pointlights[idx].ambient.xyz;
    vec3 light_diffuse = ubo_pointlights[idx].diffuse.xyz;

    float attenuation_constant = ubo_pointlights[idx].attenuation.x;
    float attentuation_linear = ubo_pointlights[idx].attenuation.y;
    float attentuation_quadratic = ubo_pointlights[idx].attenuation.z;


    float d = distance(position, light_position);

    vec3 frag_to_light = normalize(light_position - position);
    float diffuse_f = max(dot(normal, frag_to_light), 0.0);

    vec3 halfway_dir = normalize(frag_to_light + view_dir);  
    float specular_f = pow(max(dot(normal, halfway_dir), 0.0), spec_exponent);

    float attenuation = 1.0 / (
          attenuation_constant
        + attentuation_linear * d
        + attentuation_quadratic * d*d
    );

    vec3 ambient  = attenuation * albedo * light_ambient;
    vec3 diffuse  = attenuation * albedo * diffuse_f * light_diffuse;
    vec3 specular = attenuation * albedo * specular_f * 255*spec;

    vec3 result = ambient + diffuse + specular;

    return result;
}



vec3 spotlight_contribution( int idx, vec3 view_dir, vec3 position,
                             vec3 normal, vec3 albedo, float spec,
                             float spec_exponent )
{
    SpotLight light = ubo_spotlights[idx];

    vec3 light_position = light.position.xyz;
    vec3 light_direction = light.direction.xyz;

    vec3 light_ambient = light.ambient.xyz;
    vec3 light_diffuse = light.diffuse.xyz;

    float light_attenuation_constant = light.attenuation.x;
    float light_attentuation_linear = light.attenuation.y;
    float light_attentuation_quadratic = light.attenuation.z;

    float light_inner_cutoff = light.cutoff.x;
    float light_outer_cutoff = light.cutoff.y;


    float d = distance(position, light_position);

    vec3 frag_to_light = normalize(light_position - position);

    float theta     = dot(frag_to_light, normalize(-light_direction));
    float epsilon   = light_outer_cutoff - light_inner_cutoff;
    float intensity = clamp((theta - light_outer_cutoff) / epsilon, 0.0, 1.0); 

    float diffuse_f = max(dot(normal, frag_to_light), 0.0);

    vec3 halfway_dir = normalize(frag_to_light + view_dir);  
    float specular_f = pow(max(dot(normal, halfway_dir), 0.0), spec_exponent);

    float attenuation = 1.0 / (
          light_attenuation_constant
        + light_attentuation_linear * d
        + light_attentuation_quadratic * d*d
    );

    vec3 ambient  = attenuation * albedo * light_ambient;
    vec3 diffuse  = attenuation * intensity * albedo * diffuse_f * light_diffuse;
    vec3 specular = attenuation * intensity * albedo * specular_f * 255*spec;

    vec3 result = ambient + diffuse + specular;

    return result;
}



float dirlight_shadow(int idx, vec3 position)
{
    vec4 fragpos_lightspace = ubo_dirlight_lightspacematrices[0] * vec4(position, 1.0);

    vec3 projCoords = fragpos_lightspace.xyz / fragpos_lightspace.w;
    projCoords = projCoords * 0.5 + 0.5; 

    float closestDepth = texture(un_dirlight_depthmaps[0], projCoords.xy).r;   
    float currentDepth = projCoords.z;

    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 0.0 : 1.0;

    return shadow;
}



vec3 dirlight_contribution( int idx, vec3 view_dir, vec3 position,
                            vec3 normal, vec3 albedo, float spec,
                            float spec_exponent )
{
    DirLight light = ubo_dirlights[idx];

    vec3 light_direction = light.direction.xyz;
    vec3 light_ambient = light.ambient.xyz;
    vec3 light_diffuse = light.diffuse.xyz;

    vec3 frag_to_light = normalize(-light_direction);
    float diffuse_f = max(dot(normal, frag_to_light), 0.0);

    vec3 halfway_dir = normalize(frag_to_light + view_dir);  
    float specular_f = pow(max(dot(normal, halfway_dir), 0.0), spec_exponent);

    vec3 ambient  = albedo * light_ambient;
    vec3 diffuse  = albedo * diffuse_f * light_diffuse;
    vec3 specular = albedo * specular_f * 255*spec;

    float shadow = dirlight_shadow(idx, position);
    vec3 result = ambient + shadow*(diffuse + specular);

    return result; 
}

