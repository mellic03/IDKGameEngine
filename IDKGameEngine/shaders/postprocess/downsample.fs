#version 440 core

layout (location = 0) out vec4 fsout_frag_color;

in vec2 fsin_texcoords;
uniform sampler2D un_texture_0;

void main()
{
  vec2 tsize = textureSize(un_texture_0, 0);

  float offset_x = (1.0 / tsize.x);
  float offset_y = (1.0 / tsize.y);
  vec2 offsets[4] = vec2[](
    vec2( 0,         offset_y),
    vec2( 0,        -offset_y),
    vec2(-offset_x,  0),
    vec2( offset_x,  0)
  );

  float ImageKernel[4] = float[](
    1.0/4.0, // top-left
    1.0/4.0, // top-right
    1.0/4.0, // bottom-left
    1.0/4.0  // bottom-right    
  );

  vec3 sampleTex[4];
  for(int i = 0; i < 4; i++)
    sampleTex[i] = vec3(texture(un_texture_0, fsin_texcoords.st + offsets[i]));

  vec3 RenderWithKernel = vec3(0.0);

  for(int i = 0; i < 4; i++)
    RenderWithKernel += sampleTex[i] * ImageKernel[i];


  fsout_frag_color = vec4(RenderWithKernel, 1.0);
}
