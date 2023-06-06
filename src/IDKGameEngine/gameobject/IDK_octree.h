#pragma once
#include "../../IDKcore/IDKcore.h"


class idk::octree
{
private:
                struct Node;
    Node *      _root;
    void        _insert(Node* &node, int id);
    int         _relative_octant(const idk::vec4 &a, const idk::vec4 &b);

public:
                octree(): _root(nullptr) {  };
                octree(ptr_Allocator<GameObject::Physical *> &alloc): _root(nullptr) {  };
    
    void        insert(int id);

};


struct idk::octree::Node
{
    Node **     children;
    int         object_id;

    void        subdivide();

    Node(int id): object_id(id)
    {
        children = new Node *[8];
        for (int i=0; i<8; i++)
            children[i] = nullptr;
    };
};

