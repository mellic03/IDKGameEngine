#include "idk_glshaderfactory.hpp"


// #version 440 core

// layout (location = 0) out vec4 fsout_frag_color;


// #define MAX_POINTLIGHTS 10
// #define MAX_SPOTLIGHTS  10
// #define MAX_DIRLIGHTS   10

// #define NUM_POINTLIGHTS 0
// #define NUM_SPOTLIGHTS  0
// #define NUM_DIRLIGHTS   0


// in vec2 fsin_texcoords;

// uniform sampler2D un_texture_0;
// uniform sampler2D un_texture_1;
// uniform sampler2D un_texture_2;
// uniform sampler2D un_texture_3;


// layout (std140, binding = 2) uniform UBO_camera_data
// {
//     mat4 un_view;
//     mat4 un_projection;
//     vec3 un_viewpos;
// };

// struct DirLight
// {
//     vec4 direction;
//     vec4 ambient;
//     vec4 diffuse;
// };
// layout (std140, binding = 5) uniform UBO_dirlights
// {
//     DirLight    un_dirlights[MAX_DIRLIGHTS];
//     mat4        un_dirlight_matrices[MAX_DIRLIGHTS];
// };
// uniform sampler2D   un_dirlight_depthmaps[MAX_DIRLIGHTS];


// struct PointLight
// {
//     vec4 position;
//     vec4 ambient;
//     vec4 diffuse;
//     vec4 attenuation;
// };
// layout (std140, binding = 3) uniform UBO_pointlights
// {
//     PointLight  ubo_pointlights[MAX_POINTLIGHTS];
// };


