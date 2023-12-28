#version 460 core

layout (location = 0) in vec3 vsin_pos;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec3 vsin_tangent;
layout (location = 3) in vec2 vsin_texcoords;
layout (location = 4) in mat4 vsin_model;

out vec3 fsin_fragpos;
out vec3 fsin_normal;
out vec2 fsin_texcoords;


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
    mat4 model = vsin_model;

    vec4 worldpos =  model * vec4(vsin_pos, 1.0);

    float height = (mat3(model) * vsin_pos).y;
    float time   = un_camera.position.w;

    worldpos.z  += height * 0.5 * sin(3.0*time + 0.25*worldpos.z);
    worldpos.y  += height * 0.05 * sin(3.0*time + 0.25*worldpos.x);


    vec2 dir = normalize(worldpos.xz - un_viewpos.xz);

    float d = distance(un_viewpos.xz, worldpos.xz);
    float displacement = 1.0 / (1.0 + 20.0 * d*d);
          displacement = clamp(height * displacement, 0.0, 0.5);

    worldpos.xz += 0.5 * displacement * dir;
    worldpos.y -= displacement;


    fsin_fragpos = worldpos.xyz;
    fsin_normal  = vec3(0.0, 1.0, 0.0); // inverse(mat3(un_view)) * vsin_normal;
    fsin_texcoords = vsin_texcoords;


    gl_Position = un_projection * un_view * worldpos;
}