#include "IDK_octree.h"



/** Return the octant of a relative to b (0 - 7) */
int
idk::octree::_relative_octant(const idk::vec4 &a, const idk::vec4 &b)
{
    bool xn = a.x < b.x;
    bool yn = a.y < b.y;
    bool zn = a.z < b.z;

    bool xp = a.x >= b.x;
    bool yp = a.y >= b.y;
    bool zp = a.z >= b.z;

    bool octants[8] = {
        xn && yn && zn,
        xn && yn && zp,
        xn && yp && zn,
        xn && yp && zp,
        xp && yn && zn,
        xp && yn && zp,
        xp && yp && zn,
        xp && yp && zp
    };

    for (int i=0; i<8; i++)
        if (octants[i])
            return i;
}


void
idk::octree::_insert(Node* &node, int id)
{
    // int oct = _relative_octant(objects[id].pos, objects[node.object_id].pos);

    int oct = 0;
    if (node->children[oct] == nullptr)
    {
        node->children[oct] = new Node(id);
    }
}


void
idk::octree::insert(int id)
{
    if (_root == nullptr)
    {
        _root = new Node(id);
    }

    else
    {
        _insert(_root, id);
    }
}


