#include "IDKengine.h"

void renderCube();
void saveCubemap( std::string filepath, size_t w, GLint level, GLuint cubemap );

std::vector<std::string> faces
{
    "px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png"
};


int main( int argc, char **argv )
{
    const size_t      input_w    = std::atol(argv[1]);
    const size_t      output_w   = std::atol(argv[2]); // 128
    const std::string output_dir = std::string(argv[3]);


    idk::RenderEngine ren;
    ren.init("Specular IBL", 64, 64, idk::RenderEngine::INIT_FRAMEBUFFERS);
    ren.createProgram("specularIBL", "./", "specularIBL.vs", "specularIBL.fs");


    // Load input cubemap
    // -----------------------------------------------------------------------------------------
    idk::glColorConfig cubeconfig = {
        .internalformat = GL_RGBA16F,
        .format         = GL_RGBA,
        .minfilter      = GL_LINEAR,
        .magfilter      = GL_LINEAR,
        .datatype       = GL_UNSIGNED_BYTE,
        .genmipmap      = true
    };
    
    GLuint env_map = idk::gltools::loadCubemap("./input/", faces, cubeconfig);
    // -----------------------------------------------------------------------------------------


    // Render prefilter map
    // -----------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] = 
    {
        glm::lookAt(glm::vec3(0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };


    idk::glShader &program = ren.getProgram("specularIBL");

    idk::glFramebuffer framebuffer;
    idk::glColorConfig config = {
        .internalformat = GL_RGBA16F,
        .minfilter      = GL_LINEAR_MIPMAP_LINEAR,
        .magfilter      = GL_LINEAR
    };

    uint maxMipLevels = 5;
    uint mipWidth = output_w;

    for (uint mip=0; mip<=maxMipLevels; mip++)
    {
        framebuffer.reset(mipWidth, mipWidth, 1);
        framebuffer.cubemapColorAttachment(config);
        framebuffer.bind();

        program.bind();
        program.set_samplerCube("un_env_map", env_map);
        program.set_mat4("un_projection", captureProjection);
        program.set_float("un_resolution", (float)input_w);


        float roughness = (float)mip / (float)(maxMipLevels - 1);
        program.set_float("un_roughness", roughness);

        for (uint i=0; i<6; ++i)
        {
            program.set_mat4("un_view", captureViews[i]);

            idk::gl::framebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                framebuffer.cubemap_attachment,
                0
            );

            framebuffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            idk::gl::disable(GL_CULL_FACE);
            renderCube();
        }

        saveCubemap(output_dir + std::to_string(mip), mipWidth, 0, framebuffer.cubemap_attachment);

        mipWidth /= 2;
    }


    framebuffer.unbind();
    idk::glShader::unbind();
    // -----------------------------------------------------------------------------------------


    return 0;
}



void saveCubemap( std::string filepath, size_t w, GLint level, GLuint cubemap )
{
    size_t width = w;
    size_t size = 4*width*width*sizeof(GL_UNSIGNED_BYTE);
    uint8_t *data = new uint8_t[size];

    idk::gl::activeTexture(GL_TEXTURE_CUBE_MAP);
    idk::gl::bindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

    for (int i=0; i<6; i++)
    {
        GLCALL(glGetTexImage(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            level,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data
        );)

        SDL_Surface *img = SDL_CreateRGBSurfaceWithFormat(0, width, width, 8, SDL_PIXELFORMAT_RGBA32);
        SDL_LockSurface(img);

        for (int y = 0; y < width; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                Uint32 pixel = 0;
        
                pixel |= ((Uint32)data[4*(y*width + x) + 0]) << 0;  // Red
                pixel |= ((Uint32)data[4*(y*width + x) + 1]) << 8;  // Green
                pixel |= ((Uint32)data[4*(y*width + x) + 2]) << 16; // Blue
                pixel |= (0b11111111) << 24; // Alpha

                *((Uint32*)((Uint8*)(img->pixels) + y * img->pitch + x * 4)) = pixel;
            }
        }
        SDL_UnlockSurface(img);

        IMG_SavePNG(img, (filepath + faces[i]).c_str());

        SDL_FreeSurface(img);
    }
}


unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}