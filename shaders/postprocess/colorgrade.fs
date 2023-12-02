#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;


struct Camera
{
    vec4 position;
    vec4 beg;
    vec4 aberration_rg;
    vec4 aberration_b;
    vec4 exposure;
};

layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
    Camera un_camera;
};


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


const mat3 ACESInputMat = mat3(
    0.59719, 0.35458, 0.04823,
    0.07600, 0.90834, 0.01566,
    0.02840, 0.13383, 0.83777
);

// ODT_SAT => XYZ => D60_2_D65 => sRGB
const mat3 ACESOutputMat = mat3(
     1.60475, -0.53108, -0.07367,
    -0.10208,  1.10813, -0.00605,
    -0.00327, -0.07276,  1.07602
);

vec3 RRTAndODTFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786) - 0.000090537;
    vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
    return a / b;
}

vec3 ACESFitted(vec3 color)
{
    color = color * ACESInputMat;

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = color * ACESOutputMat;

    // Clamp to [0, 1]
    color = clamp(color, 0.0, 1.0);

    return color;
}



#define MIN_EXPOSURE 0.85
#define MAX_EXPOSURE 1.50
#define AUTO_EXPOSURE 0

#if AUTO_EXPOSURE == 1
    float autoExposure()
    {
        vec2 size = textureSize(un_texture_0, 0);
        float miplevel = floor(log2(max(size.x, size.y))) + 1.0;

        vec3 max_rgb = textureLod(un_texture_0, fsin_texcoords, miplevel).rgb;
        float brightness = dot(max_rgb, vec3(0.2126, 0.7152, 0.0722));
        
        return clamp(0.5 / brightness, MIN_EXPOSURE, MAX_EXPOSURE);
    }
#else
    float autoExposure()
    {
        return un_camera.exposure.x;
    }
#endif


void main()
{
    float exposure = autoExposure();

    vec3 hdr   = textureLod(un_texture_0, fsin_texcoords, 0.0).rgb;
    vec3 sdr   = aces(exposure * hdr);
    vec3 color = pow(sdr, vec3(1.0 / un_camera.beg.y));
  
    fsout_frag_color = vec4(color, 1.0);
}
