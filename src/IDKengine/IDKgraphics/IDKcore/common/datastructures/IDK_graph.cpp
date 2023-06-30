#include "IDK_graph.h"
#include "IDK_heap.h"

#include <cmath>
#include <iostream>


idk::graph::graph(size_t num_vertices)
{
    _adj_matrix.resize(num_vertices);
    for (size_t i=0; i<num_vertices; i++)
    {
        _adj_matrix[i].resize(num_vertices);
        for (size_t j=0; j<num_vertices; j++)
            _adj_matrix[i][j] = INFINITY;
    }
}


void
idk::graph::add_edge(int a, int b, float weight)
{
    _adj_matrix[a][b] = weight;
}


idk::vector<int>
idk::graph::ucs(int a, int b)
{
    idk::vector<int> path;


    return path;
}


idk::vector<int>
idk::graph::dijkstra(int a, int b)
{   
    // idk::vector<float> dist(_adj_matrix.size(), INFINITY);
    // idk::vector<int> prev(_adj_matrix.size(), -1);
    // dist[a] = 0.0f;

    // auto cmp = [](const idk::pair<float *, int> &a, const idk::pair<float *, int> &b)
    // { return *(a.first) < *(b.first); };
    // idk::heap<idk::pair<float *, int>> pq( cmp );

    // for (size_t i=0; i<_adj_matrix.size(); i++)
    //     pq.insert({&dist[i], i});
   
    
    // clock_t time = 0;
    // while(pq.empty() == false)
    // {
    //     int u = pq.pop().second;

    //     for (size_t v = 0; v < _adj_matrix.size(); v++)
    //     {
    //         float weight = _adj_matrix[u][v];
            
    //         if (weight == INFINITY)
    //             continue;

    //         if (dist[u] + weight < dist[v])
    //         {
    //             dist[v] = dist[u] + weight;
    //             prev[v] = u;
    //             clock_t t = clock();
    //             pq.reheap();
    //             time += clock() - t;
    //         }
    //     }
    // }
    // std::cout << "dtime: " << ((double)time) / CLOCKS_PER_SEC << std::endl;

    idk::vector<int> path;
    // int idx = b;
    // while (idx != -1)
    // {
    //     path.push(idx);
    //     idx = prev[idx];
    // }

    return path;
}


idk::vector<int>
idk::graph::Astar(int a, int b)
{
    idk::vector<int> path;


    return path;
}

