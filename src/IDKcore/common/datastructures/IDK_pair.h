#pragma once

#include "../IDKdecl.h"


template <typename T, typename U>
struct idk::pair
{
    T       first;
    U       second;
            pair() {  };
            pair(T f, U s): first(f), second(s) {  };
};
