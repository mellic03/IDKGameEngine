
// vec3 calculate_pointlight(PointLight light, vec3 albedo, vec3 fragPos, vec3 normal, float spec_strength, float spec_exponent)
// {
//     float d = length(light.position - fragPos);
//     if (d > light.radius)
//         return vec3(0.0);

//     vec3 lightDir = normalize(light.position - fragPos);
//     vec3 viewDir = normalize(viewPos - fragPos);

//     float diff = max(dot(normal, lightDir), 0.0);

//     vec3 halfwayDir = normalize(lightDir + viewDir);  
//     float spec = pow(max(dot(normal, halfwayDir), 0.0), spec_exponent);

//     float attenuation = 1.0 / (light.constant + d*light.linear + d*d*light.quadratic);
//     attenuation *= 1.0 - d/light.radius;

//     vec3 diffuse  = attenuation * albedo * diff * light.diffuse;
//     vec3 ambient  = attenuation * albedo * light.ambient;
//     vec3 specular = attenuation * albedo * spec * light.diffuse * spec_strength;

//     return  (ambient + diffuse + specular);
// }