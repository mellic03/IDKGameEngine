#pragma once

#include <glm/glm.hpp>

namespace idk
{
    // Templated functions --------------------------------
    template <typename T> void      dowith_probability(float p, T lambda);
    template <typename T> void      swap(T &a, T &b);
    template <typename T> T         min(T a, T b);
    template <typename T> T         max(T a, T b);
    template <typename T> T         clamp(T value, T min, T max);
    // --------------------------------------------------

    float                           distanceSq(glm::vec3 &a, glm::vec3 &b);
};


template <typename T>
void
idk::dowith_probability(float p, T lambda)
{
    if ((rand()%1000) / 1000.0f < p)
    {
        lambda();
    }
}


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



