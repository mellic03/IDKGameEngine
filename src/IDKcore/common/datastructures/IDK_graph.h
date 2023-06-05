#pragma once

#include "../IDKdecl.h"
#include "IDK_vector.h"
#include "IDK_pair.h"

class idk::graph
{
private:
    idk::vector<idk::vector<idk::pair<int, float>>> _adj_list;

public:
                            graph() {  };
    idk::vector<int>        ucs(int a, int b);
    idk::vector<int>        dijkstra(int a, int b);
    idk::vector<int>        Astar(int a, int b);
};

