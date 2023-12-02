#version 330 core

out vec4 fsout_fragcolor;
in vec3 fsin_fragpos;

uniform samplerCube un_env_map;
#define PI 3.14159265359


vec3 correct( vec3 color )
{
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
    return color;
}

void main()
{
    vec3 N = normalize(fsin_fragpos);
    vec3 irradiance = vec3(0.0);


    // tangent space calculation from origin point
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up         = normalize(cross(N, right));

    float sampleDelta = 0.025;
    float nrSamples = 0.0;

    for (float phi = 0.0;  phi < 2.0*PI;  phi += sampleDelta)
    {
        for (float theta = 0.0;  theta < 0.5*PI;  theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

            irradiance += (texture(un_env_map, sampleVec).rgb) * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    fsout_fragcolor = vec4(irradiance, 1.0);
}
