#pragma once

#define IDK_DECL

namespace IDK
{
    // Data structures ----------------------------------
    template <typename T> class Allocator;
    template <typename T> class ptr_Allocator;

    template <typename T> class vector;

    template <typename T, int size> class vec_t;
    typedef vec_t<float, 2> vec2;
    typedef vec_t<float, 3> vec3;
    typedef vec_t<float, 4> vec4;

    typedef vec_t<int, 2> ivec2;
    typedef vec_t<int, 3> ivec3;
    typedef vec_t<int, 4> ivec4;

    template <int size> class mat_t;
    typedef mat_t<3> mat3;
    typedef mat_t<4> mat4;

    template <typename T, int size>
    T *value_ptr(IDK::vec_t<T, size> &);
    
    template <int size>
    float *value_ptr(IDK::mat_t<size> &);


    class transform;
    // --------------------------------------------------


    // Graphics-level -----------------------------------
    struct vertex;
    class Shader;
    class Mesh;
    class Material;
    class Model;
    class Camera;
    class RenderEngine;
    // --------------------------------------------------


    // Engine-level-------------------------------------
    class Keylog;
    class Engine;

    namespace GameObject
    {
        class Base;
    }
    // --------------------------------------------------
};