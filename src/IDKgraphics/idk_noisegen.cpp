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

glm::vec3 dirvec_to_closest(glm::vec3 point, std::vector<glm::vec3> points)
{
    float min_dist = INFINITY;
    glm::vec3 min_vec = glm::vec3(INFINITY);

    for (glm::vec3 p: points)
    {
        float dist = glm::distance(point, p);

        if (dist < min_dist)
        {
            min_dist = dist;
            min_vec = p - point;
        }
    }

    return min_vec;
}


GLuint
idk::noisegen3D::worley( int w )
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);


    const int num_points = 25;

    std::vector<glm::vec3> points(num_points);
    for (int i=0; i<num_points; i++)
    {
        float x = (float)(rand() % (w*1000)) / 1000.0f;
        float y = (float)(rand() % (w*1000)) / 1000.0f;
        float z = (float)(rand() % (w*1000)) / 1000.0f;

        points[i] = glm::vec3(x, y, z);
    }

    float *texture_data = new float[w*w*w];

    float max_value = 0.0f;

    for (int z=0; z<w; z++)
    {
        for (int y=0; y<w; y++)
        {
            for (int x=0; x<w; x++)
            {
                float dist = dist_to_closest(glm::vec3(x, y, z), points);
                if (dist > max_value)
                    max_value = dist;
                texture_data[z*w*w + y*w + x] = dist;
            }
        }
    }

    for (int z=0; z<w; z++)
    {
        for (int y=0; y<w; y++)
        {
            for (int x=0; x<w; x++)
            {
                texture_data[z*w*w + y*w + x] /= max_value;
                texture_data[z*w*w + y*w + x] = 1.0f - texture_data[z*w*w + y*w + x];
            }
        }
    }

    gl::texImage3D(GL_TEXTURE_3D, 0, GL_R32F, w, w, w, 0, GL_RED, GL_FLOAT, texture_data);

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
idk::noisegen3D::worleyvec( int w )
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_3D, texture);


    const int num_points = 25;

    std::vector<glm::vec3> points(num_points);
    for (int i=0; i<num_points; i++)
    {
        float x = (float)(rand() % (w*1000)) / 1000.0f;
        float y = (float)(rand() % (w*1000)) / 1000.0f;
        float z = (float)(rand() % (w*1000)) / 1000.0f;

        points[i] = glm::vec3(x, y, z);
    }

    glm::vec3 *data = new glm::vec3[w*w*w];

    float max_value = 0.0f;

    for (int z=0; z<w; z++)
    {
        for (int y=0; y<w; y++)
        {
            for (int x=0; x<w; x++)
            {
                glm::vec3 dir = dirvec_to_closest(glm::vec3(x, y, z), points);
                data[z*w*w + y*w + x] = dir;

                float dist = glm::length(dir);
                if (dist > max_value)
                {
                    max_value = dist;
                }
            }
        }
    }

    for (int z=0; z<w; z++)
    {
        for (int y=0; y<w; y++)
        {
            for (int x=0; x<w; x++)
            {
                data[z*w*w + y*w + x] /= max_value;
                data[z*w*w + y*w + x] = 1.0f - data[z*w*w + y*w + x];
            }
        }
    }


    glTexImage3D( GL_TEXTURE_3D, 0, GL_R32F, w, w, w, 0, GL_RED, GL_FLOAT, data );

    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    idk::gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

    glGenerateMipmap(GL_TEXTURE_3D);
    glBindTexture(GL_TEXTURE_3D, 0);

    delete[] data;

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
                int r = (rand() % 1000000);
                int g = (rand() % 1000000);
                int b = (rand() % 1000000);

                data[z*h*w + y*w + x].r = (r / 1000000.0f);
                data[z*h*w + y*w + x].g = (g / 1000000.0f);
                data[z*h*w + y*w + x].b = (b / 1000000.0f);

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

    gl::texImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, w, h, d, 0, GL_RGBA, GL_FLOAT, data);

    gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gl::texParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

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



