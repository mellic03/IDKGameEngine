#pragma once

#include <cstddef>
#include <cmath>


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
    class skiplist;

    class graph;
    // --------------------------------------------------


    // Graphics-level -----------------------------------
    // Implemented in IDKgraphics/ 
    class Transform;
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
    class octree;
    class Keylog;
    class Controller;
    class Engine;
    class ComponentSystem;

    enum class MouseButton { LEFT, MIDDLE, RIGHT };
    // --------------------------------------------------


};

