#version 460 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;


void main()
{
    vec2 tsize = textureSize(un_texture_0, 2);
    float offset_x = 1.0 * (1.0 / tsize.x);
    float offset_y = 1.0 * (1.0 / tsize.y);

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


    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(un_texture_0, fsin_texcoords.st + offsets[i]));
    }

    vec3 maxvalue = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        vec3 value = sampleTex[i] * kernel[i];
        if (length(value) > length(maxvalue))
        {
            maxvalue = value;
        }
    }

    fsout_frag_color = vec4(5*maxvalue, 1.0);
}

