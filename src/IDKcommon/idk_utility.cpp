#include "idk_utility.h"

int
idk::roundup( int n, int multipleof )
{
    if (n % multipleof == 0)
        return n;
    return n + (multipleof - (n % multipleof));
};
