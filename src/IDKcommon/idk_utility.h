#pragma once

#include <glm/glm.hpp>

#include <iostream>
#include <string>


namespace idk
{
    template <typename T> T         min( T, T );
    template <typename T> T         max( T, T );
    template <typename T> T         clamp( T, T, T );
    template <typename T> void      swap( T&, T& );
                          int       roundup( int n, int multipleof );
                          float     distanceSq( glm::vec3 a, glm::vec3 b );


    template <typename T, typename... Args>
    void    print( std::string sep, T head, Args... );
    void    print( std::string sep );

};


template <typename T>
T
idk::min( T a, T b )
{
    return (a < b) ? a : b;
}


template <typename T>
T
idk::max( T a, T b )
{
    return (a > b) ? a : b;
}


template <typename T>
T
idk::clamp( T in_value, T min_value, T max_value )
{
    return idk::max(min_value, idk::min(in_value, max_value));    
}


template <typename T> void
idk::swap( T&a, T&b )
{
    T temp = a;
    a = b;
    b = temp;
}



template <typename T, typename... Args> void
idk::print( std::string sep, T head, Args... rest )
{
    std::cout << head << sep;
    idk::print(sep, rest...);
}

