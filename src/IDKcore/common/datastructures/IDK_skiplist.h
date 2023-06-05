#pragma once

#include "../IDKdecl.h"


template <typename key_t>
class idk::skiplist
{
private:
                        struct Node;
    Node *              _head;
    Node *              _tail;
    double              _prob;

    void                _insert(Node* &node, key_t key, std::string &data);
    void                _remove(Node* &node, key_t key);

public:
                        skiplist();


    void                insert(key_t key, std::string data)  { _insert(_head, key, data); };
    void                remove(key_t key)                    { _remove(_head, key);       };

};


template <typename key_t>
struct idk::skiplist<key_t>::Node
{
    key_t               key;
    std::string         data;
    idk::vector<Node>   forward;
    Node(key_t k, std::string d): key(k), data(d) {  };
};


template <typename key_t>
idk::skiplist<key_t>::skiplist(): _prob(0.5)
{
    _head = new Node(std::numeric_limits<int>::max(), "head");
}


template <typename key_t>
void
idk::skiplist<key_t>::_insert(Node* &node, key_t key, std::string &data)
{

}


template <typename key_t>
void
idk::skiplist<key_t>::_remove(Node* &node, key_t key)
{

}


