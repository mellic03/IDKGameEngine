#include "idk_shadowcascade.hpp"



const std::vector<glm::vec4> &
idk::glDepthCascade::get_corners( const glm::mat4 &P, const glm::mat4 &V )
{
    static std::vector<glm::vec4> corners(8);
    corners.clear();

    glm::mat4 inv = glm::inverse(P * V);

    for (uint x=0; x<2; ++x)
    {
        for (uint y=0; y<2; ++y)
        {
            for (uint z=0; z<2; ++z)
            {
                glm::vec4 pt = inv * glm::vec4(
                    2.0f*x - 1.0f,
                    2.0f*y - 1.0f,
                    2.0f*z - 1.0f,
                    1.0f
                );

                corners.push_back(pt / pt.w);
            }
        }
    }

    return corners;
}


const glm::vec3 &
idk::glDepthCascade::get_center( const std::vector<glm::vec4> &corners )
{
    static glm::vec3 center;
    center = glm::vec3(0.0f);

    for (const glm::vec4 &v: corners)
    {
        center += glm::vec3(v);
    }

    center /= 8.0f;

    return center;
}


const glm::mat4 &
idk::glDepthCascade::get_view( const glm::vec3 &L, const std::vector<glm::vec4> &corners )
{
    static glm::mat4 view;

    const auto &center = get_center(corners);

    view = glm::lookAt(
        center - L,
        center,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    return view;
}


const glm::mat4 &
idk::glDepthCascade::get_projection( const float texture_width,
                                     const glm::vec3 &L,
                                     const glm::mat4 &V,
                                     const std::vector<glm::vec4> &corners )
{
    static glm::mat4 P;

    glm::vec3 minv = glm::vec3(+INFINITY);
    glm::vec3 maxv = glm::vec3(-INFINITY);

    for (glm::vec4 v: corners)
    {
        v = V * v;
        
        minv.x = glm::min(minv.x, v.x);
        minv.y = glm::min(minv.y, v.y);
        minv.z = glm::min(minv.z, v.z);
        maxv.x = glm::max(maxv.x, v.x);
        maxv.y = glm::max(maxv.y, v.y);
        maxv.z = glm::max(maxv.z, v.z);
    }

    float w_texelspace = fabs(maxv.x - minv.x) / texture_width;
    float h_texelspace = fabs(maxv.y - minv.y) / texture_width;

    minv.x = w_texelspace * glm::floor(minv.x / w_texelspace);
    maxv.x = w_texelspace * glm::floor(maxv.x / w_texelspace);
    minv.y = h_texelspace * glm::floor(minv.y / h_texelspace);
    maxv.y = h_texelspace * glm::floor(maxv.y / h_texelspace);

    constexpr float zMult = 2.0f;
    minv.z *= (minv.z < 0) ? zMult : 1.0f/zMult;
    maxv.z *= (maxv.z < 0) ? 1.0f/zMult : zMult;

    P = glm::ortho(
        minv.x, maxv.x, minv.y, maxv.y, minv.z, maxv.z
    ) * V;

    return P;
}




void
idk::glDepthCascade::reset( int w, int h )
{
    f_reset(w, h, 0);

    static const idk::DepthAttachmentConfig config = {
        .internalformat = GL_DEPTH_COMPONENT,
        .datatype       = GL_FLOAT
    };

    depthArrayAttachment(NUM_CASCADES+1, config);
}


void
idk::glDepthCascade::setCascadeDepths( float a, float b, float c, float d )
{
    m_cascade_depths = glm::vec4(a, b, c, d);
}


void
idk::glDepthCascade::setOutputAttachment( GLint idx )
{
    gl::namedFramebufferTextureLayer(
        m_FBO,
        GL_DEPTH_ATTACHMENT,
        depth_attachment,
        0,
        idx
    );
}


GLuint
idk::glDepthCascade::getTextureArray()
{
    return depth_attachment;
}


const std::vector<glm::mat4> &
idk::glDepthCascade::computeCascadeMatrices( float cam_fov,  float cam_aspect,
                                             float cam_near, float cam_far,
                                             const glm::mat4 &cam_view,
                                             const glm::vec3 &light_dir )
{
    m_cascade_matrices.resize(0);

    float near = cam_near;
    float far  = cam_far;

    for (int i=0; i<NUM_CASCADES; i++)
    {
        far = m_cascade_depths[i] * cam_far;

        glm::mat4 P = glm::perspective(
            cam_fov, cam_aspect, near, far
        );

        near = far;


        const auto &corners    = get_corners(P, cam_view);
        const auto &view       = get_view(light_dir, corners);
        const auto &projection = get_projection(size().x, light_dir, view, corners);

        m_cascade_matrices.push_back(projection);
    }

    return m_cascade_matrices;
}


