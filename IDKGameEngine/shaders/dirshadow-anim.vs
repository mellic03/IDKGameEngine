#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;
layout (location = 4) in ivec4 vsin_bone_ids;
layout (location = 5) in vec4  vsin_bone_weights;

uniform mat4 un_lightspacematrix;
uniform mat4 un_model;

// uniform mat4 un_bonetransforms[50];
layout (std140, binding = 6) uniform UBO_armature
{
    mat4 un_bonetransforms[50];
};


void main()
{
    mat4 accum_transform = mat4(0.0);

    for (int i=0; i<4; i++)
    {
        if (vsin_bone_ids[i] == -1) 
        {
            break;
        }

        float weight    = vsin_bone_weights[i];
        mat4  transform = un_bonetransforms[vsin_bone_ids[i]];
        accum_transform += weight * transform;
    }

    gl_Position = un_lightspacematrix * un_model * accum_transform * vec4(vsin_pos, 1.0);
}  



