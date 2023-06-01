#pragma once

namespace IDK
{
    // Data structures ----------------------------------
    template <typename T> class Allocator;
    template <typename T> class ptr_Allocator;

    template <typename T> class vector;

    template <int size> class vec_t;
    typedef vec_t<2> vec2;
    typedef vec_t<3> vec3;
    typedef vec_t<4> vec4;

    template <int size> class mat_t;
    typedef mat_t<3> mat3;
    typedef mat_t<4> mat4;

    template <int size> float *value_ptr(IDK::vec_t<size> &);
    template <int size> float *value_ptr(IDK::mat_t<size> &);


    class transform;
    // --------------------------------------------------


    // Graphics-level -----------------------------------
    class Shader;
    class Texture;
    class Material;
    class Model;
    class ModelSystem;
    class Camera;
    class RenderEngine;
    class GraphicsEngine;
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