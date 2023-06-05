#include "IDK_graph.h"


idk::vector<int>
idk::graph::ucs(int a, int b)
{
    idk::vector<int> path;


    return path;
}


idk::vector<int>
idk::graph::dijkstra(int a, int b)
{    
    idk::vector<float> dist(_adj_list.size());
    idk::vector<int> prev(_adj_list.size());
    idk::vector<pair<float, int>> pq(_adj_list.size());

    pq.push({0.0f, a});

    while (pq.empty() == false)
    {
        int u = pq.pop().second;
        
        for (idk::pair<int, float> &p: _adj_list[u])
        {
            int v = p.first;
            float weight = p.second;

            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[u], v});
            }
        }
    }

    idk::vector<int> path;
    int idx = b;
    while (idx != -1)
    {
        path.push(idx);
        idx = prev[idx];
    }

    // path.reverse();

    return path;
}


idk::vector<int>
idk::graph::Astar(int a, int b)
{
    idk::vector<int> path;


    return path;
}

