
vec3 pointlight_contribution(int idx, vec3 view_dir, vec3 albedomap, vec3 specularmap, float spec_exponent)
{
    vec3 light_position = ubo_pointlight_position[idx].xyz;

    vec3 light_ambient = ubo_pointlight_ambient[idx].xyz;
    vec3 light_diffuse = ubo_pointlight_ambient[idx].xyz;

    float attenuation_constant = ubo_pointlight_attenuation[idx].x;
    float attentuation_linear = ubo_pointlight_attenuation[idx].y;
    float attentuation_quadratic = ubo_pointlight_attenuation[idx].z;


    float d = distance(fsin_fragpos, light_position);

    vec3 frag_to_light = normalize(light_position - fsin_fragpos);
    float diffuse_f = max(dot(fsin_normal, frag_to_light), 0.0);

    vec3 halfway_dir = normalize(frag_to_light + view_dir);  
    float specular_f = pow(max(dot(fsin_normal, halfway_dir), 0.0), spec_exponent);

    float attenuation = 1.0 / (
          attenuation_constant
        + attentuation_linear * d
        + attentuation_quadratic * d*d
    );

    vec3 ambient  = attenuation * albedomap * light_ambient;
    vec3 diffuse  = attenuation * albedomap * diffuse_f * light_diffuse;
    vec3 specular = attenuation * albedomap * specular_f * 255*specularmap;

    vec3 result = ambient + diffuse + specular;

    return result;
}



vec3 spotlight_contribution(int idx, vec3 view_dir, vec3 albedomap, vec3 specularmap, float spec_exponent)
{
    vec3 light_position = ubo_spotlight_position[idx].xyz;
    vec3 light_direction = normalize(ubo_spotlight_direction[idx].xyz);

    vec3 light_ambient = ubo_spotlight_ambient[idx].xyz;
    vec3 light_diffuse = ubo_spotlight_diffuse[idx].xyz;

    float light_attenuation_constant = ubo_spotlight_attenuation[idx].x;
    float light_attentuation_linear = ubo_spotlight_attenuation[idx].y;
    float light_attentuation_quadratic = ubo_spotlight_attenuation[idx].z;

    float light_inner_cutoff = ubo_spotlight_cutoff[idx].x;
    float light_outer_cutoff = ubo_spotlight_cutoff[idx].y;


    float d = distance(fsin_fragpos, light_position);

    vec3 frag_to_light = normalize(light_position - fsin_fragpos);

    float theta     = dot(frag_to_light, normalize(-light_direction));
    float epsilon   = light_outer_cutoff - light_inner_cutoff;
    float intensity = clamp((theta - light_outer_cutoff) / epsilon, 0.0, 1.0); 

    float diffuse_f = max(dot(fsin_normal, frag_to_light), 0.0);

    vec3 halfway_dir = normalize(frag_to_light + view_dir);  
    float specular_f = pow(max(dot(fsin_normal, halfway_dir), 0.0), spec_exponent);

    float attenuation = 1.0 / (
          light_attenuation_constant
        + light_attentuation_linear * d
        + light_attentuation_quadratic * d*d
    );

    vec3 ambient  = attenuation * albedomap * light_ambient;
    vec3 diffuse  = attenuation * intensity * albedomap * diffuse_f * light_diffuse;
    vec3 specular = attenuation * intensity * albedomap * specular_f * 255*specularmap;

    vec3 result = ambient + diffuse + specular;

    return result;
}