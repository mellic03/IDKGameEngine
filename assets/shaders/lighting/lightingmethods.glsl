vec3 pointlight_contribution(int idx, vec3 view_dir, vec3 albedomap, vec3 specularmap, float spec_exponent)
{
    pointlight light = un_pointlights[idx];
    float d = distance(fsin_fragpos, light.position);

    vec3 frag_to_light = normalize(light.position - fsin_fragpos);
    float diffuse_f = max(dot(fsin_normal, frag_to_light), 0.0);

    vec3 halfway_dir = normalize(frag_to_light + view_dir);  
    float specular_f = pow(max(dot(fsin_normal, halfway_dir), 0.0), spec_exponent);

    float attenuation = 1.0 / (
          light.attenuation_constant
        + light.attentuation_linear * d
        + light.attentuation_quadratic * d*d
    );

    vec3 ambient  = attenuation * albedomap * light.ambient;
    vec3 diffuse  = attenuation * albedomap * diffuse_f * light.diffuse;
    vec3 specular = attenuation * albedomap * specular_f * 255*specularmap;

    vec3 result = ambient + diffuse + specular;

    return result;
}



vec3 spotlight_contribution(int idx, vec3 view_dir, vec3 albedomap, vec3 specularmap, float spec_exponent)
{
    spotlight light = un_spotlights[idx];
    float d = distance(fsin_fragpos, light.position);

    vec3 frag_to_light = normalize(light.position - fsin_fragpos);

    float theta     = dot(frag_to_light, normalize(-light.direction));
    float epsilon   = light.outer_cutoff - light.inner_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0); 

    float diffuse_f = max(dot(fsin_normal, frag_to_light), 0.0);

    vec3 halfway_dir = normalize(frag_to_light + view_dir);  
    float specular_f = pow(max(dot(fsin_normal, halfway_dir), 0.0), spec_exponent);

    float attenuation = 1.0 / (
          light.attenuation_constant
        + light.attentuation_linear * d
        + light.attentuation_quadratic * d*d
    );

    vec3 ambient  = attenuation * albedomap * light.ambient;
    vec3 diffuse  = attenuation * intensity * albedomap * diffuse_f * light.diffuse;
    vec3 specular = attenuation * intensity * albedomap * specular_f * 255*specularmap;

    vec3 result = ambient + diffuse + specular;

    return result;
}
