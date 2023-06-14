#include "IDK_skiplist.h"


idk::skiplist::skiplist(): _prob(0.5)
{
    _head = new Node(std::numeric_limits<int>::max(), "head");
}


void
idk::skiplist::_insert(Node* &node, int key, std::string &data)
{

}


void
idk::skiplist::_remove(Node* &node, int key)
{

}


