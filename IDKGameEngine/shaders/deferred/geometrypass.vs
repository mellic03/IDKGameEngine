#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;
out vec3 fsin_normal;
out vec2 fsin_texcoords;

out vec3 TBN_viewpos;
out vec3 TBN_fragpos;
out mat3 TBN;
out mat3 TBNT;

uniform mat4 un_model;

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


void main()
{
    mat4 model = un_model;

    vec4 worldpos = model * vec4(vsin_pos, 1.0);

    fsin_fragpos = worldpos.xyz;
    fsin_normal  = (model * vec4(vsin_normal, 0.0)).xyz;
    fsin_texcoords = vsin_texcoords;


    vec3 N = normalize(mat3(model) * normalize(vsin_normal));
    vec3 T = normalize(mat3(model) * normalize(vsin_tangent));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    B = normalize(B - dot(B, N) * N);

    TBN  = mat3(T, B, N);
    TBNT = transpose(TBN);
    TBN_fragpos = TBNT * fsin_fragpos;
    TBN_viewpos = TBNT * un_viewpos;

    gl_Position = un_projection * un_view * worldpos;
}