#pragma once

#include <cstddef>


#define IDK_DECL

namespace idk
{
    // Data structures ----------------------------------
    // Implemented in IDKcore/common/datastructures 
    template <typename T> class Allocator;

    template <typename T, typename U> struct pair;
    template <typename T, typename U, typename V> struct triple;

    template <typename T> class stack;
    template <typename T> class vector;
    template <typename T> class queue;
    template <typename T> class heap;
    template <typename key_t, typename data_t> class BST;
    template <typename T> class quadtree;
    template <typename T> class trie;
    template <typename T> class linkedlist;
    template <typename key_t> class skiplist;

    class graph;
    // --------------------------------------------------


    // Graphics-level -----------------------------------
    // Implemented in IDKgraphics/ 
    class transform;
    struct vertex;
    class glUniforms;
    class glInterface;
    class Mesh;
    class Material;
    class Model;
    class Camera;
    namespace lightsource
    {
        struct Point;
        struct Spot;
        struct Dir;
    };
    class RenderEngine;
    // --------------------------------------------------


    // Engine-level-------------------------------------
    // Implemented in IDKGameEngine/ 
    enum class ECScomponent
    {
        render          =  1 << 0,
        phyiscs         =  1 << 1,
        num_components  =  2
    };
    class GameObject;
    class octree;
    class Keylog;
    class Controller;
    class Engine;
    class Module;

    namespace builtin_modules
    {
        class Builtin_Physics;
        class Builtin_PlayerControl;
        class Builtin_UI;
    };

    // --------------------------------------------------


    // Utility functions --------------------------------
    // Implemented below
    template <typename T>       void        swap(T &a, T &b);
    template <typename T>       T           min(T a, T b);
    template <typename T>       T           max(T a, T b);
    template <typename T>       T           clamp(T value, T min, T max);
    template <typename T>       T *         raw_ptr(idk::vector<T> &vec);
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
T
idk::min(T a, T b)
{
    return (a < b) ? a : b;
}


template<typename T>
T
idk::max(T a, T b)
{
    return (a > b) ? a : b;
}


template<typename T>
T
idk::clamp(T value, T min, T max)
{
    return idk::max(min, idk::min(value, max));
}


template <typename T>
T *
idk::raw_ptr(idk::vector<T> &vec)
{
    return &vec[0];
}


