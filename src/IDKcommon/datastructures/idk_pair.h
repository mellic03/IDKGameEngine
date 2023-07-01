#pragma once

#include "datastructures_decl.h"

template <typename T, typename U>
struct idk::pair
{
    T       first;
    U       second;
            pair() {  };
            pair(T f, U s): first(f), second(s) {  };
};

template <typename T, typename U, typename V>
struct idk::triple
{
    T       first;
    U       second;
    V       third;
            triple() {  };
            triple(T f, U s, V t): first(f), second(s), third(t) {  };
};
