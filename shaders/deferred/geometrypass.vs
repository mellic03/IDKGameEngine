#version 440 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec2 vsin_texcoords;

out vec3 fsin_fragpos;
out vec3 fsin_normal;
out vec2 fsin_texcoords;

out vec4 fsin_fragpos_dirlightspace[10];

uniform mat4 un_model;
uniform mat4 un_dirlight_lightspacematrices[10];

layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};

void main()
{
    fsin_fragpos = (un_model * vec4(vsin_pos, 1.0)).xyz;
    fsin_normal = normalize(mat3(un_model) * vsin_normal);
    fsin_texcoords = vsin_texcoords;

    for (int i=0; i<10; i++)
    {
        fsin_fragpos_dirlightspace[i] = un_dirlight_lightspacematrices[i] * vec4(vsin_pos, 1.0);
    }

    gl_Position = un_projection * un_view * un_model * vec4(vsin_pos, 1.0);
}
