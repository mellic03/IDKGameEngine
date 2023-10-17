#include "idk_noisegen.hpp"

#include "IDKglInterface/IDKglInterface.h"
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


