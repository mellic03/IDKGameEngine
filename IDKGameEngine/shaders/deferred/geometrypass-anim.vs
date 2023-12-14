#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;
layout (location = 4) in ivec4 vsin_bone_ids;
layout (location = 5) in vec4  vsin_bone_weights;


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


// uniform mat4 un_bonetransforms[50];
layout (std140, binding = 6) uniform UBO_armature
{
    mat4 un_bonetransforms[50];
};



void main()
{
    vec4 anim_fragpos = vec4(0.0);
    vec4 anim_normal  = vec4(0.0);
    vec4 anim_tangent = vec4(0.0);

    for (int i=0; i<4; i++)
    {
        if (vsin_bone_ids[i] == -1) 
        {
            if (i == 0)
            {
                anim_fragpos = vec4(vsin_pos, 1.0);
            }

            break;
        }

        float weight    = vsin_bone_weights[i];
        mat4  transform = un_bonetransforms[vsin_bone_ids[i]];

        anim_fragpos += weight * transform * vec4(vsin_pos, 1.0);
        anim_normal  += weight * transform * vec4(vsin_normal, 0.0);
        anim_tangent += weight * transform * vec4(vsin_tangent, 0.0);
    }
    anim_fragpos.w = 1.0;
    anim_normal.w  = 0.0;
    anim_tangent.w = 0.0;

    vec3 N = normalize((un_model * anim_normal)).xyz;
    vec3 T = normalize((un_model * anim_tangent)).xyz;
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);


    fsin_fragpos = (un_model * anim_fragpos).xyz;
    fsin_normal  = N;
    fsin_texcoords = vsin_texcoords;

    TBN  = mat3(T, B, N);
    TBNT = transpose(TBN);
    TBN_fragpos = TBNT * fsin_fragpos;
    TBN_viewpos = TBNT * un_camera.position.xyz;

    gl_Position = un_projection * un_view * vec4(fsin_fragpos, 1.0);
}
