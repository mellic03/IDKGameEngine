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


const std::vector<idk::Frustum> &
idk::Frustum::getSubfrusta()
{
    return m_subfrusta;
}


const glm::mat4 &
idk::Frustum::getView( const glm::vec3 &L )
{
    m_view = glm::lookAt(
        m_center + L,
        m_center,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    return m_view;
}


const glm::mat4 &
idk::Frustum::getProjection( const glm::vec3 &L )
{
    m_view = getView(L);

    glm::vec3 minv = glm::vec3(+INFINITY);
    glm::vec3 maxv = glm::vec3(-INFINITY);

    for (glm::vec4 v: m_corners)
    {
        v = m_view * v;
        
        minv.x = glm::min(minv.x, v.x);
        minv.y = glm::min(minv.y, v.y);
        minv.z = glm::min(minv.z, v.z);
        maxv.x = glm::max(maxv.x, v.x);
        maxv.y = glm::max(maxv.y, v.y);
        maxv.z = glm::max(maxv.z, v.z);
    }

    constexpr float zMult = 10.0f;
    if (minv.z < 0)
    {
        minv.z *= zMult;
    }
    else
    {
        minv.z /= zMult;
    }
    if (maxv.z < 0)
    {
        maxv.z /= zMult;
    }
    else
    {
        maxv.z *= zMult;
    }
    
    m_projection = glm::ortho(
        minv.x, maxv.x, minv.y, maxv.y, minv.z, maxv.z
    );

    return m_projection;
}



