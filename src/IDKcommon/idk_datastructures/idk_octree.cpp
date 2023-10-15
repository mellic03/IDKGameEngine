#include "idk_octree.hpp"



idk::SVOctree::SVOctree()
{
    m_root_id = m_nodes.create();
}


void
idk::SVOctree::f_insert(int node_id, glm::vec3 pos, glm::vec3 color )
{

}


void
idk::SVOctree::insert( glm::vec3 position, glm::vec3 color )
{
    f_insert(m_root_id, position, color);
}




