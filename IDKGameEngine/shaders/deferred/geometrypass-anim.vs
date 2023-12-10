#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;
layout (location = 4) in ivec4 vsin_bone_ids;
layout (location = 5) in vec4  vsin_bone_weights;

uniform mat4 un_bonetransforms[35];

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
    vec4 fragpos = vec4(0.0);

    for (int i=0; i<4; i++)
    {
        if (vsin_bone_ids[i] == -1) 
        {
            continue;
        }

        if (vsin_bone_ids[i] >= 30) 
        {
            fragpos = vec4(vsin_pos, 1.0);
            break;
        }

        vec4 localPosition = un_bonetransforms[vsin_bone_ids[i]] * vec4(vsin_pos, 1.0);
        fragpos += localPosition * vsin_bone_weights[i];
    }

    mat4 model = un_model;
    fragpos = model * fragpos;

    fsin_fragpos = fragpos.xyz;
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

    gl_Position = un_projection * un_view * fragpos;
}
