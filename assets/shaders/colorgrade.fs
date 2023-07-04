#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_screen_texture;


vec3 filmic(vec3 x, float gamma)
{
    vec3 X = max(vec3(0.0), x - 0.004);
    vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
    return pow(result, vec3(1.0 / gamma));
}

vec3 aces(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{
    const float gamma = 2.2;
    const float exposure = 1.0;
   
    vec3 hdrColor = texture(un_screen_texture, fsin_texcoords).rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    fsout_frag_color = vec4(mapped, 1.0);
    // fsout_frag_color = vec4(0.0, 1.0, 0.0, 1.0);
}
