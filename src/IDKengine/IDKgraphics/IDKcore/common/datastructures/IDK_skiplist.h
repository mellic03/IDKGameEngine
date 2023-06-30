#pragma once

#include "../IDKdecl.h"
#include <vector>
#include <string>

class idk::skiplist
{
private:
                        struct Node;
    Node *              _head;
    Node *              _tail;
    double              _prob;

    void                _insert(Node* &node, int key, std::string &data);
    void                _remove(Node* &node, int key);

public:
                        skiplist();


    void                insert(int key, std::string data)  { _insert(_head, key, data); };
    void                remove(int key)                    { _remove(_head, key);       };

};


struct idk::skiplist::Node
{
    int                 key;
    std::string         data;
    std::vector<Node>   forward;
    Node(int k, std::string d): key(k), data(d) {  };
};


