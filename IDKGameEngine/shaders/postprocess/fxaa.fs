#version 460 core

out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;


// FXAA Quality Settings
#define FXAA_SPAN_MAX (8.0)
#define FXAA_REDUCE_MUL (1.0/8.0)
#define FXAA_REDUCE_MIN (1.0/128.0)

vec3 fxaa(sampler2D tex, vec2 frameBufSize, vec2 tex_coords)
{
    vec3 result = vec3(0.0);

    vec3 rgbNW = texture(tex, tex_coords+(vec2(-1.0,-1.0)/frameBufSize)).xyz;
    vec3 rgbNE = texture(tex, tex_coords+(vec2(1.0,-1.0)/frameBufSize)).xyz;
    vec3 rgbSW = texture(tex, tex_coords+(vec2(-1.0,1.0)/frameBufSize)).xyz;
    vec3 rgbSE = texture(tex, tex_coords+(vec2(1.0,1.0)/frameBufSize)).xyz;
    vec3 rgbM  = texture(tex, tex_coords).xyz;

    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
        FXAA_REDUCE_MIN);

    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
            max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
            dir * rcpDirMin)) / frameBufSize;

    vec3 rgbA = (1.0/2.0) * (
        texture(tex,  tex_coords.xy + dir * (1.0/3.0 - 0.5)).xyz +
        texture(tex,  tex_coords.xy + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
        texture(tex,  tex_coords.xy + dir * (0.0/3.0 - 0.5)).xyz +
        texture(tex,  tex_coords.xy + dir * (3.0/3.0 - 0.5)).xyz);
    float lumaB = dot(rgbB, luma.xyz);

    if((lumaB < lumaMin) || (lumaB > lumaMax))
    {
        result.xyz=rgbA;
    }
    else
    {
        result.xyz=rgbB;
    }

    return result;
}


void main()
{
    // vec3 color = texture(un_texture_0, fsin_texcoords).rgb;
    vec3 color = fxaa(un_texture_0, textureSize(un_texture_0, 0), fsin_texcoords);

    fsout_frag_color = vec4(color, 1.0);
}
