#pragma once

#include "idk_allocator.h"


namespace idk { class SVOctree; };


class idk::SVOctree
{
private:
    struct Node
    {
        bool leaf = false;
        int  children[8] = {0};
    };

    idk::Allocator<Node>    m_nodes;
    int                     m_root_id;
    int                     SPAN;

    int         f_get_octant( glm::vec3 center, glm::vec3 position );
    glm::vec3   f_shift_center( int octant, glm::vec3 center, int span );
    void        f_insert( int id, glm::ivec3 color, glm::vec3 pos, glm::vec3 center, int depth );


public:
            SVOctree( int span );

    void    insert( glm::ivec3 color, glm::vec3 center );

};




