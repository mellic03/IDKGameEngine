#version 460 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;
layout (location = 4) in ivec4 vsin_bone_ids;
layout (location = 5) in vec4  vsin_bone_weights;


uniform mat4 un_lightspacematrix;
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

    for (int i=0; i<4; i++)
    {
        if (vsin_bone_ids[i] == -1) 
        {
            break;
        }

        float weight    = vsin_bone_weights[i];
        mat4  transform = un_bonetransforms[vsin_bone_ids[i]];

        anim_fragpos += weight * transform * vec4(vsin_pos, 1.0);
    }

    gl_Position = un_lightspacematrix * un_model * vec4(anim_fragpos.xyz, 1.0);
}


