#pragma once

#include "idk_allocator.h"


namespace idk { class SVOctree; };


class idk::SVOctree
{
public:
    struct Node { int children[8] = {0}; };


private:
    idk::Allocator<Node>    m_nodes;
    int                     m_root_id;

    void    f_insert( int id, glm::vec3 pos, glm::vec3 color );


public:
            SVOctree();

    void    insert( glm::vec3 position, glm::vec3 color );

};




