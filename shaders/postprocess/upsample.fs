// Used for up/down sampling

#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;
uniform sampler2D un_texture_4; // previous (half) resolution.

void main()
{
    vec2 tsize = textureSize(un_texture_0, 0);

    float offset_x = 1.0 / tsize.x;
    float offset_y = 1.0 / tsize.y;
    vec2 offsets[9] = vec2[](
        vec2(-offset_x,  offset_y), // top-left
        vec2( 0.0f,      offset_y), // top-center
        vec2( offset_x,  offset_y), // top-right
        vec2(-offset_x,  0.0f),     // center-left
        vec2( 0.0f,      0.0f),     // center-center
        vec2( offset_x,  0.0f),     // center-right
        vec2(-offset_x, -offset_y), // bottom-left
        vec2( 0.0f,     -offset_y), // bottom-center
        vec2( offset_x, -offset_y)  // bottom-right    
    );

    float ImageKernel[9] = float[](
        1.0/16.0, // top-left
        2.0/16.0, // top-center
        1.0/16.0, // top-right
        2.0/16.0, // center-left
        4.0/16.0, // center-center
        2.0/16.0, // center-right
        1.0/16.0, // bottom-left
        2.0/16.0, // bottom-center
        1.0/16.0  // bottom-right    
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = 
        texture(un_texture_0, fsin_texcoords.st + offsets[i]).rgb
        + texture(un_texture_4, fsin_texcoords + offsets[i]/2.0).rgb;
    }

    vec3 RenderWithKernel = vec3(0.0);

    for(int i = 0; i < 9; i++)
    {
        RenderWithKernel += sampleTex[i] * ImageKernel[i];
    }

    fsout_frag_color += vec4(RenderWithKernel, 1.0);
}