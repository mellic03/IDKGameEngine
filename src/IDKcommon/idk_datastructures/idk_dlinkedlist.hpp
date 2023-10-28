#pragma once


namespace idk { template <typename T> class DLinkedList; };


template <typename T>
class idk::DLinkedList
{
private:
    struct Node
    {
        Node *prev, *next;
        T     data;

        Node( Node *p, const T &d ):  prev(p), next(nullptr), data(d) {  };
    };

    Node *m_head = nullptr;
    Node *m_tail = nullptr;


public:
    void  insert( const T & );
    // T &   find()s

};



template <typename T>
void
idk::DLinkedList<T>::insert( const T &data )
{
    if (m_head == nullptr)
    {
        m_head = new Node(nullptr, data);
        m_head->data = data;
        m_head->next = m_head;
        m_head->prev = m_head;

        m_tail = m_head;
    }

    if (m_tail == m_head)
    {
        m_tail = new Node(nullptr, data);
        m_tail->data = data;
        m_tail->prev = m_head;

        m_head->next = m_tail;
        m_head->prev = nullptr;

        return;
    }

    m_tail->next = new Node(m_tail, data);
    m_tail = m_tail->next;
};
