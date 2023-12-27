#include "idk_frustum.hpp"


const std::vector<glm::vec4> &
idk::Frustum::getCorners( const glm::mat4 &P, const glm::mat4 &V )
{
    m_corners.clear();
    glm::mat4 inv = glm::inverse(P * V);

    for (uint x=0; x<2; ++x)
    {
        for (uint y=0; y<2; ++y)
        {
            for (uint z=0; z<2; ++z)
            {
                glm::vec4 pt = inv * glm::vec4(
                    2.0f * x - 1.0f,
                    2.0f * y - 1.0f,
                    2.0f * z - 1.0f,
                    1.0f
                );

                m_corners.push_back(pt / pt.w);
            }
        }
    }

    return m_corners;
}


const glm::vec3 &
idk::Frustum::getCenter()
{
    m_center = glm::vec3(0.0f);
    for (glm::vec4 &v: m_corners)
        m_center += glm::vec3(v);
    m_center /= 8.0f;

    return m_center;
}


const glm::mat4 &
idk::Frustum::getView( const glm::vec3 &L )
{
    m_center = getCenter();

    m_view = glm::lookAt(
        m_center - L,
        m_center,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    return m_view;
}


const glm::mat4 &
idk::Frustum::getProjection( const glm::vec3 &L, const std::vector<glm::vec4> &corners )
{
    m_view = getView(L);

    glm::vec3 minv = glm::vec3(+INFINITY);
    glm::vec3 maxv = glm::vec3(-INFINITY);

    for (glm::vec4 v: corners)
    {
        v = m_view * v;
        
        minv.x = glm::min(minv.x, v.x);
        minv.y = glm::min(minv.y, v.y);
        minv.z = glm::min(minv.z, v.z);
        maxv.x = glm::max(maxv.x, v.x);
        maxv.y = glm::max(maxv.y, v.y);
        maxv.z = glm::max(maxv.z, v.z);
    }

    constexpr float TEXTURE_WIDTH = 1024.0f;

    float w_texelspace = (maxv.x - minv.x) / TEXTURE_WIDTH;
    float h_texelspace = (maxv.y - minv.y) / TEXTURE_WIDTH;

    minv.x = w_texelspace * glm::floor(minv.x / w_texelspace);
    maxv.x = w_texelspace * glm::floor(maxv.x / w_texelspace);
    minv.y = h_texelspace * glm::floor(minv.y / h_texelspace);
    maxv.y = h_texelspace * glm::floor(maxv.y / h_texelspace);

    constexpr float zMult = 4.0f;
    minv.z *= (minv.z < 0) ? zMult : 1.0f / zMult;
    maxv.z *= (maxv.z < 0) ? zMult : 1.0f / zMult;


    m_projection = glm::ortho(
        minv.x, maxv.x, minv.y, maxv.y, minv.z, maxv.z
    ) * m_view;

    return m_projection;
}



std::vector<glm::mat4>
idk::Frustum::getCascades( idk::Camera &camera, const glm::vec3 &L )
{
    constexpr int CASCADES = 4;

    std::vector<glm::mat4> cascades;

    float cam_far  = camera.farPlane();
    float cam_near = camera.nearPlane();

    float near = cam_near;
    float far  = cam_far;

    std::vector<float> C { 0.1f, 0.3f, 0.6f, 1.0f };

    for (int i=0; i<CASCADES; i++)
    {
        far = C[i] * cam_far;

        glm::mat4 P = glm::perspective(
            camera.getFOV(), camera.getAspect(),
            near, far
        );
    
        const auto &corners    = getCorners(P, camera.view());
        const auto &projection = getProjection(L, corners);

        cascades.push_back(projection);
        near = far;
    }

    return cascades;
}


