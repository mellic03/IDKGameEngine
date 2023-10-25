#include "idk_noisegen.hpp"

#include "IDKglInterface/IDKglInterface.hpp"
#include <vector>


float dist_to_closest(glm::vec3 point, std::vector<glm::vec3> points)
{
    float min_dist = INFINITY;

    for (glm::vec3 p: points)
    {
        float dist = glm::distance(point, p);

        if (dist < min_dist)
            min_dist = dist;
    }

    return min_dist;
}


GLuint
idk::noisegen3D::worley()
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);


    constexpr int num_points = 50;
    constexpr int width = 50;

    std::vector<glm::vec3> points(num_points);
    for (int i=0; i<num_points; i++)
    {
        float x = (float)(rand() % (width*1000)) / 1000.0f;
        float y = (float)(rand() % (width*1000)) / 1000.0f;
        float z = (float)(rand() % (width*1000)) / 1000.0f;

        points[i] = glm::vec3(x, y, z);
    }

    float *texture_data = new float[width*width*width];

    float max_value = 0.0f;

    for (int z=0; z<width; z++)
    {
        for (int y=0; y<width; y++)
        {
            for (int x=0; x<width; x++)
            {
                float dist = dist_to_closest(glm::vec3(x, y, z), points);
                if (dist > max_value)
                    max_value = dist;
                texture_data[z*width*width + y*width + x] = dist;
            }
        }
    }

    for (int z=0; z<width; z++)
    {
        for (int y=0; y<width; y++)
        {
            for (int x=0; x<width; x++)
            {
                texture_data[z*width*width + y*width + x] /= max_value;
                texture_data[z*width*width + y*width + x] = 1.0f - texture_data[z*width*width + y*width + x];
            }
        }
    }


    glTexImage3D( GL_TEXTURE_3D, 0, GL_R32F, width, width, width,
                         0, GL_RED, GL_FLOAT, texture_data );

    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, 0);

    delete[] texture_data;

    return texture;
}


GLuint
idk::noisegen3D::white( int w, int h, int d )
{
    srand(clock());

    glm::vec4 *data = new glm::vec4[w*h*d];

    float minf = 100000.0f;
    float maxf = -100000.0f;

    for (int z=0; z<d; z++)
    {
        for (int y=0; y<h; y++)
        {
            for (int x=0; x<w; x++)
            {
                int r = (rand() % 100000) - 50000;
                int g = (rand() % 100000) - 50000;
                int b = (rand() % 100000) - 50000;

                data[z*h*w + y*w + x].r = r / 50000.0f;
                data[z*h*w + y*w + x].g = g / 50000.0f;
                data[z*h*w + y*w + x].b = b / 50000.0f;

                minf = std::min(minf, data[z*h*w + y*w + x].r);
                minf = std::min(minf, data[z*h*w + y*w + x].g);
                minf = std::min(minf, data[z*h*w + y*w + x].b);

                maxf = std::max(maxf, data[z*h*w + y*w + x].r);
                maxf = std::max(maxf, data[z*h*w + y*w + x].g);
                maxf = std::max(maxf, data[z*h*w + y*w + x].b);
            }
        }
    }
    std::cout << minf << ", " << maxf << "\n";

    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);


    glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA16F, w, h, d, 0, GL_RGBA, GL_FLOAT, data );

    // idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, 0);

    delete[] data;

    return texture;
}




GLuint
idk::noisegen2D::white( int w, int h )
{
    glm::vec4 *data = new glm::vec4[w*h];

    // float minf = 100000.0f;
    // float maxf = -100000.0f;

    for (int y=0; y<h; y++)
    {
        for (int x=0; x<w; x++)
        {
            int r = (rand() % 100000) - 50000;
            int g = (rand() % 100000) - 50000;
            int b = (rand() % 100000) - 50000;

            data[y*w + x].r = r / 10000.0f;
            data[y*w + x].g = g / 10000.0f;
            data[y*w + x].b = b / 10000.0f;

            // minf = std::min(minf, data[y*w + x].r);
            // minf = std::min(minf, data[y*w + x].g);
            // minf = std::min(minf, data[y*w + x].b);

            // maxf = std::max(maxf, data[y*w + x].r);
            // maxf = std::max(maxf, data[y*w + x].g);
            // maxf = std::max(maxf, data[y*w + x].b);
        }
    }
    // std::cout << minf << ", " << maxf << "\n";

    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);


    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, data );

    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] data;

    return texture;
}



