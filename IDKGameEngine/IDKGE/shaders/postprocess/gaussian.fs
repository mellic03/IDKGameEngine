#version 460 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;


#define MIP_LEVEL 0
#define KSIZE 3

void main()
{
    vec2 tsize = textureSize(un_texture_0, MIP_LEVEL);
    
    float offset_x = 1.0 / tsize.x;
    float offset_y = 1.0 / tsize.y;
    vec2 offset = vec2(offset_x, offset_y);


    vec3 result = vec3(0.0);

    for (int y=-KSIZE; y<KSIZE; y++)
    {
        for (int x=-KSIZE; x<KSIZE; x++)
        {
            result += (1.0 / (KSIZE*KSIZE)) * texture(un_texture_0, fsin_texcoords + vec2(x, y)*offset).rgb;
        }
    }


    fsout_frag_color = vec4(result, 1.0);
}

