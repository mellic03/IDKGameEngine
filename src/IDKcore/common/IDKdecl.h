#pragma once

#include <cstddef>


#define IDK_DECL

namespace idk
{
    // Data structures ----------------------------------
    template <typename T> class Allocator;
    template <typename T> class ptr_Allocator;

    template <typename T, typename U> struct pair;
    template <typename T, typename U, typename V> struct triple;

    template <typename T> class stack;
    template <typename T> class vector;
    template <typename T> class heap;
    template <typename key_t, typename data_t> class BST;
    template <typename T> class quadtree;
    template <typename T> class trie;
    template <typename T> class linkedlist;
    template <typename key_t> class skiplist;

    // template <typename T, int size> class vec_t;
    // typedef vec_t<float, 2> vec2;
    // typedef vec_t<float, 3> vec3;
    // typedef vec_t<float, 4> vec4;

    // typedef vec_t<int, 2> ivec2;
    // typedef vec_t<int, 3> ivec3;
    // typedef vec_t<int, 4> ivec4;

    // template <int size> class mat_t;
    // typedef mat_t<3> mat3;
    // typedef mat_t<4> mat4;

    // template <typename T, int size>
    // T *raw_ptr(idk::vec_t<T, size> &);
    
    // template <int size>
    // float *raw_ptr(idk::mat_t<size> &);

    class graph;
    // --------------------------------------------------


    // Graphics-level -----------------------------------
    class transform;
    struct vertex;
    class glInterface;
    class glShader;
    class Mesh;
    class Material;
    class Model;
    class Camera;
    class RenderEngine;
    // --------------------------------------------------


    // Engine-level-------------------------------------
    namespace GameObject
    {
        class Base;
        class Renderable;
        class Physical;
    }
    class octree;
    class Keylog;
    class Navmap;
    class Engine;
    class Module;
    // --------------------------------------------------


    // Utility functions --------------------------------
    template<typename T>        void        swap(T &a, T &b);
    template<typename T>        T &         min(T &a, T &b);
    template<typename T>        T &         max(T &a, T &b);
    // --------------------------------------------------

};


template<typename T>
void
idk::swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
T &
idk::min(T &a, T &b)
{
    return (a < b) ? a : b;
}

template<typename T>
T &
idk::max(T &a, T &b)
{
    return (a > b) ? a : b;
}

