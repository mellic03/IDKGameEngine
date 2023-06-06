#pragma once

#include "../IDKdecl.h"
#include "IDK_vector.h"
#include "IDK_pair.h"

class idk::graph
{
private:
    idk::vector<idk::vector<float>> _adj_matrix;
    idk::vector<idk::vector<bool>> _con_matrix;

public:
                            graph(size_t num_vertices);

    void                    add_edge(int a, int b, float weight);

    idk::vector<int>        ucs(int a, int b);
    idk::vector<int>        dijkstra(int a, int b);
    idk::vector<int>        Astar(int a, int b);
};

