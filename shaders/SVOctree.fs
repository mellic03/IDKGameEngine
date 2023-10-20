#version 440 core

layout (location = 0) out vec4 fsout_frag_color;
layout (location = 1) out vec4 fsout_frag_depth;

in vec2 fsin_texcoords;


layout (std140, binding = 2) uniform UBO_camera_data
{
    mat4 un_view;
    mat4 un_projection;
    vec3 un_viewpos;
};

uniform sampler2D un_texture_1;

uniform vec3 un_viewdir;
uniform float un_fov;
uniform vec2 un_resolution;


struct OctNode_data
{
    int blocktype;
    float span;
};

struct OctNode
{
    int leaf, blocktype, pad1, pad2;
    int  children[8];
};

layout (std430, binding = 3) buffer SVO_nodes
{
    OctNode svo_nodes[];
};


#define BLOCK_GRASS 1
#define BLOCK_RED   2

vec3 svo_blocktype_color( int blocktype )
{
    switch (blocktype)
    {
        case BLOCK_GRASS: return vec3(0.2, 1.0, 0.2);
        case BLOCK_RED:   return vec3(1.0, 0.2, 0.2);
    }
}


float ray_plane_intersection( vec3 ray_pos, vec3 ray_dir, vec3 plane_pos, vec3 plane_dir )
{
    const float EPSILON = 1e-6;

    float denom = dot(ray_dir, plane_dir);

    if (denom < EPSILON)
    {
        return 0.0;
    }

    float t = dot(plane_pos - ray_pos, plane_dir) / denom;

    return t;
}


vec3 next_step( vec3 ray_pos, vec3 ray_dir, float span )
{
    vec3 ray_step = vec3(0.0);

    float t = ray_plane_intersection(ray_pos, ray_dir, vec3(0.0), vec3(0.0));
    if (t < 0.0)
    {
        discard;
    }

    vec3 intersection = ray_pos + t * normalize(ray_dir);

    return ray_step;
}


int svo_get_octant( vec3 pos, vec3 center )
{
    int octant = 0;

    if (pos.x < center.x) octant |= 1;
    if (pos.y < center.y) octant |= 2;
    if (pos.z < center.z) octant |= 4;

    return octant;
}


vec3 svo_shift_center( int octant, vec3 center, float span )
{
    vec3 offset;
    
    offset.x = (octant & 1) == 0 ? span/4.0 : -span/4.0;
    offset.y = (octant & 2) == 0 ? span/4.0 : -span/4.0;
    offset.z = (octant & 4) == 0 ? span/4.0 : -span/4.0;

    return center + offset;
}


OctNode_data svo_get_node( vec3 pos )
{
    OctNode_data Odata;

    OctNode node = svo_nodes[0];
    vec3 center  = vec3(0.0);
    float span   = 32.0;
    float min_span = 1.0/4.0;

    while (node.leaf == 0)
    {
        int octant = svo_get_octant(pos, center);

        // This absolutely must be checked first   
        if (node.children[octant] == -1)
        {
            break;
        }

        node = svo_nodes[node.children[octant]];

        center = svo_shift_center(octant, center, span);
        span /= 2.0;
    }

    Odata.span = span;
    Odata.blocktype = node.blocktype;

    return Odata;
}


float trace_shadow( vec3 view_pos, vec3 ray_pos )
{
    vec3 ray_dir = vec3(1.0, 1.0, 1.0);

    // Cast ray to light source
    for (int i=0; i<25; i++)
    {
        ray_pos += 0.15*ray_dir;

        OctNode_data node = svo_get_node(ray_pos);
        float span = node.span;

        if (node.blocktype > 0)
        {
            return 0.5;
        }
    }

    return 1.0;
}


vec4 trace( vec3 view_pos, vec3 ray_pos, vec3 ray_dir )
{
    vec3 result = vec3(0.0, 0.0, 0.0);
    float depth = 0.0;

    for (int i=0; i<300; i++)
    {
        ray_pos += 0.1*ray_dir;

        if (distance(ray_pos, vec3(0.0)) > 20.0)
        {
            return vec4(result, 1000000.0);
        }

        OctNode_data node = svo_get_node(ray_pos);
        vec3 color = svo_blocktype_color(node.blocktype);
        float span = node.span;

        if (node.blocktype > 0)
        {
            result = color;
            depth = distance(view_pos, ray_pos);

            float shadow = trace_shadow(view_pos, ray_pos);

            return vec4(shadow*result, depth);
        }
    }


    return vec4(result, 10000.0);
}



void main()
{
    vec3 frag_pos = texture(un_texture_1, fsin_texcoords).xyz;

    vec3 ray_pos = un_viewpos;
    vec3 ray_dir = normalize(frag_pos - un_viewpos);

    vec4 color_depth = trace(un_viewpos, ray_pos, ray_dir);
    vec3 color = color_depth.rgb;
    float depth = color_depth.w;

    fsout_frag_color = vec4(color, 1.0);
    fsout_frag_depth = vec4(vec3(depth), 1.0);
}