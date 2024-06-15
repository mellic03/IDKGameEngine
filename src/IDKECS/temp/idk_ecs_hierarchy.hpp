#pragma once

#include <libidk/idk_vector.hpp>
#include <libidk/idk_stack.hpp>
#include <libidk/idk_allocator.hpp>
#include <libidk/idk_resource_allocator.hpp>
#include <libidk/idk_export.hpp>

#include <algorithm>


namespace idk
{
    class SceneNode;
    class RootSceneNode;
    class SceneHierarchy;
};


class IDK_VISIBLE idk::SceneNode
{
private:
    static constexpr size_t MAX_CHILDREN = 8;

public:
    static inline idk::block_allocator<SceneNode, 512> allocator;
    // static inline idk::stack<int> IDstack = idk::stack<int>(512);

    int obj_id;

    SceneNode *parent = nullptr;
    SceneNode *children[MAX_CHILDREN];


    void *operator new( size_t size, void *ptr ) noexcept
    {
        return ptr;
    };

    void *operator new ( size_t size )
    {
       return reinterpret_cast<void *>(allocator.allocate(1));
    };

    void operator delete ( void *p )
    {
        allocator.deallocate(reinterpret_cast<SceneNode *>(p), 1);
    };



    SceneNode( int id = -1, SceneNode *parent_node = nullptr )
    : obj_id(id), parent(parent_node)
    {
        std::fill_n(children, MAX_CHILDREN, nullptr);
        
        if (parent)
        {
            parent->addChild(this);
        }
    };


    ~SceneNode()
    {
        for (SceneNode *child: children)
        {
            if (child != nullptr)
            {
                child->parent = this->parent;
            }
        }

        if (parent != nullptr)
        {
            parent->removeChild(this);
        }
    };


    void addChild( SceneNode *child )
    {
        for (int i=0; i<MAX_CHILDREN; i++)
        {
            if (children[i] == nullptr)
            {
                children[i] = child;
                return;
            }
        }

        IDK_ASSERT("Too many children!", false);
    }


    void removeChild( SceneNode *child )
    {
        for (int i=0; i<MAX_CHILDREN; i++)
        {
            if (children[i] == child)
            {
                children[i] = nullptr;
            }
        }

        IDK_ASSERT("Child does not exist!", false);
    };

};



class IDK_VISIBLE idk::RootSceneNode
{
public:
    idk::vector<idk::SceneNode *> children;


    void addChild( SceneNode *node )
    {
        children.push_back(node);
    }

    void removeChild( SceneNode *child )
    {
        int idx = 0;

        for (idx=0; idx<children.size(); idx++)
        {
            if (children[idx] == child)
            {
                break;
            }
        }

        if (idx == children.size())
        {
            return;
        }

        std::swap(children[idx], children.back());
        delete children.back();
        children.pop_back();
    };

};



class IDK_VISIBLE idk::SceneHierarchy
{
private:
    using Node = idk::SceneNode;

    idk::stack<int>     m_IDstack;
    RootSceneNode       m_root;


    int popID()
    {
        int id = m_IDstack.top();
                 m_IDstack.pop();
    
        return id;
    };

    void pushID( int id )
    {
        m_IDstack.push(id);
    };


    Node *find( Node *node, int obj_id )
    {
        if (node->obj_id == obj_id)
        {
            return node;
        }

        for (auto *child: node->children)
        {
            if (child != nullptr)
            {
                return find(child, obj_id);
            }
        }

        return nullptr;
    };


    Node *find( int obj_id )
    {
        for (Node *child: m_root.children)
        {
            Node *node = find(child, obj_id);
            
            if (node)
            {
                return node;
            }
        }

        IDK_ASSERT("Object does not exist!", false);
    };



public:

    SceneHierarchy( size_t max_objects )
    {
        for (size_t i=0; i<max_objects; i++)
        {
            m_IDstack.push(max_objects-1 - i);
        }
    };


    void giveChild( int parent, int child )
    {
        Node *pnode = find(parent);
        Node *cnode = find(child);

        if (pnode == nullptr || cnode == nullptr)
        {
            return;
        }

        pnode->addChild(cnode);
    };


    void removeChild( int parent, int child )
    {
        Node *pnode = find(parent);
        Node *cnode = find(child);

        if (pnode == nullptr || cnode == nullptr)
        {
            return;
        }
    
        cnode->parent = pnode->parent;
        pnode->removeChild(cnode);
    };


    int createGameObject( int parent = -1 )
    {
        IDK_ASSERT("Over budget!", m_IDstack.size() > 0);

        int obj_id = popID();

        if (parent == -1)
        {
            m_root.addChild(new Node(obj_id, nullptr));
        }

        else
        {
            Node *pnode = find(parent);
            IDK_ASSERT("Parent does not exist", pnode != nullptr);
            new Node(obj_id, pnode);
        }

        return obj_id;
    };


    void destroyGameObject( int obj_id )
    {
        Node *node = find(obj_id);
        IDK_ASSERT("Object does not exist", node != nullptr);
        delete node;
        pushID(obj_id);
    };


    void createNode( int parent = -1 )
    {
        IDK_ASSERT("Over budget!", m_IDstack.size() > 0);
        m_root.addChild(new Node(popID(), find(parent)));
    };


    bool hasParent( int obj_id )
    {
        Node *node = find(obj_id);
        return node->parent != nullptr;
    };


    // bool hasParent  ( int child );
    // void giveParent ( int parent, int child ) { giveChild(parent, child); };
    // void giveChild  ( int parent, int child );

    // void removeParent   ( int child );
    // void removeChild    ( int parent, int child );
    // void removeChildren ( int parent );


    // int         getParent   ( int child);
    // const auto &getChildren ( int parent ) { return m_children[parent]; };

};
