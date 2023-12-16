#pragma once

#include "idk_camera.hpp"


namespace idk { class Frustum; };


class idk::Frustum
{
private:
    std::vector<glm::vec4>  m_corners;
    glm::vec3               m_center;
    glm::mat4               m_view;
    glm::mat4               m_projection;

public:
    const std::vector<glm::vec4> &getCorners( const glm::mat4 &P, const glm::mat4 &V );
    const glm::vec3              &getCenter();
    const glm::mat4              &getView( const glm::vec3 &L );
    const glm::mat4              &getProjection( const glm::vec3 &L, const std::vector<glm::vec4> &corners );

    std::vector<glm::mat4>        getCascades( idk::Camera &camera, const glm::vec3 &L );

    static std::vector<glm::mat4> getCascades2( idk::Camera &camera, const glm::vec3 &L );
};





