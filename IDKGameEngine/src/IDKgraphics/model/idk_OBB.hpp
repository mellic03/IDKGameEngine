#pragma once

#include <libidk/GL/common.hpp>


namespace idk
{
    struct OBB;

    glm::vec4 world_to_NDC( const glm::vec4 & );
    idk::OBB  OBB_to_NDC( const glm::mat4 &proj_view, const idk::OBB & );
};

struct idk::OBB
{
    OBB() {  };
    OBB( const glm::vec2 &x, const glm::vec2 &y, const glm::vec2 &z );

    glm::vec4 corners[8];
};

