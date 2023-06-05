#pragma once

#include "../IDKdecl.h"
#include <string>
#include <iostream>


template <typename key_t, typename data_t>
class idk::BST
{
private:
                        struct Node;
    Node *              _root;

    void                _free(Node* &node);
    void                _insert(Node* &node, key_t key, data_t &data);
    void                _remove(Node* &node, key_t key);
    void                _print(std::string prefix, Node* &node, bool is_right);

public:
                        BST(): _root(nullptr) {  };
                        ~BST();

    void                insert(key_t key, data_t data);
    void                remove(key_t key)   { _remove(_root, key);      };
    void                print()             { _print("", _root, false); };
};


template <typename key_t, typename data_t>
struct idk::BST<key_t, data_t>::Node
{
    key_t key;
    data_t data;
    Node *left, *right;
    Node(key_t k, data_t d): key(k), data(d), left(nullptr), right(nullptr) {  }; 
};


template <typename key_t, typename data_t>
void
idk::BST<key_t, data_t>::_free(Node* &node)
{
    if (node == nullptr)
        return;

    _free(node->left);
    _free(node->right);

    delete node;
}


template <typename key_t, typename data_t>
idk::BST<key_t, data_t>::~BST()
{
    _free(_root);
}


template <typename key_t, typename data_t>
void
idk::BST<key_t, data_t>::_insert(Node* &node, key_t key, data_t &data)
{
    if (key < node->key)
    {
        if (node->left == nullptr)
            node->left = new Node(key, data);
        else
            _insert(node->left, key, data);
        return;
    }

    if (key > node->key)
    {
        if (node->right == nullptr)
            node->right = new Node(key, data);
        else
            _insert(node->right, key, data);
        return;
    }
}


template <typename key_t, typename data_t>
void
idk::BST<key_t, data_t>::insert(key_t key, data_t data)
{
    if (_root == nullptr)
    {
        _root = new Node(key, data);
        return;
    }
    _insert(_root, key, data);
}


template <typename key_t, typename data_t>
void
idk::BST<key_t, data_t>::_remove(Node* &node, key_t key)
{
    if (key == node->key)
    {

    }

    if (key < node->key)
    {

    }

    if (key > node->key)
    {

    }

}



template <typename key_t, typename data_t>
void
idk::BST<key_t, data_t>::_print( std::string prefix, Node* &node, bool is_right )
{
    if (node == nullptr)
    {
        std::cout << prefix << (is_right ? " ├───X" : " └───X" ) << std::endl;
        return;
    }
    std::cout << prefix;
    std::cout << (is_right ? " ├── " : " └── " );
    std::cout << node->key << ": " << node->data << std::endl;

    _print( prefix + (is_right ? " │  " : "    "), node->right, true);
    _print( prefix + (is_right ? " │  " : "    "), node->left, false);
}
