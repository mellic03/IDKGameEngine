#pragma once

namespace idk
{
    template <typename T> T         min( T, T );
    template <typename T> T         max( T, T );
    template <typename T> T         clamp( T, T, T );

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

