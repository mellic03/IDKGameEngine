#include "idk_svoctree.hpp"



idk::SVOctree::SVOctree( int span ): SPAN(span)
{
    m_root_id = m_nodes.create();
}


int
idk::SVOctree::f_get_octant( glm::vec3 pos, glm::vec3 center )
{
    constexpr int OCTANTS[2][2][2] = {
        {{0, 1}, {2, 3}},
        {{4, 5}, {6, 7}}
    };

    return OCTANTS[pos.y>center.y][pos.z>center.z][pos.x>center.x];
}


glm::vec3
idk::SVOctree::f_shift_center( int octant, glm::vec3 center, int span )
{
    // constexpr float Y_OFFSETS[2][2][2] = {
    //     {{+1, +1}, {+1, +1}},
    //     {{-1, -1}, {-1, -1}}
    // };

    // constexpr float Z_OFFSETS[2][2][2] = {
    //     {{-1, -1}, {+1, +1}},
    //     {{-1, -1}, {+1, +1}}
    // };

    // constexpr float X_OFFSETS[2][2][2] = {
    //     {{-1, +1}, {-1, +1}},
    //     {{-1, +1}, {-1, +1}}
    // };

    constexpr glm::vec3 OFFSETS[8] = {
        {-1.0f, +1.0f, -1.0f}, {+1.0f, +1.0f, -1.0f}, {-1.0f, +1.0f, +1.0f}, {+1.0f, +1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f}, {+1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, +1.0f}, {+1.0f, -1.0f, +1.0f}
    };

    return center + (span/4.0f) * OFFSETS[octant];
}



void
idk::SVOctree::f_insert( int id, glm::ivec3 color, glm::vec3 pos, glm::vec3 center, int depth )
{
    const int span = SPAN / depth;
    if (span <= 1)
    {
        return;
    }

    const int octant = f_get_octant(pos, center);
    Node &node = m_nodes.get(id);

    if (node.children[octant] == -1)
    {
        node.children[octant] = m_nodes.create();
    }

    const glm::vec3 new_center = f_shift_center(octant, center, span);
    f_insert(node.children[octant], color, pos, new_center, depth+1);

}


void
idk::SVOctree::insert( glm::ivec3 color, glm::vec3 position )
{
    f_insert(m_root_id, color, position, glm::vec3(0.0f), 1);
}


