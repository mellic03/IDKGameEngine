#pragma once

#include "datastructures_decl.h"
#include "idk_vector.h"

#include <set>
#include <map>
#include <limits>


#define adjlist_t std::map<int, std::map<int, float>>

template <typename T>
class idk::Graph
{
private:
    adjlist_t           m_edges;
    std::map<int, T>    m_vertices;

public:
                        Graph() {  };

    void                addVertex( int v, const T &data );
    const T &           getVertex( int v );

    void                addEdge( int v0, int v1, float weight );
    void                removeEdge( int v0, int v1 );

    void                shortestPath(int start, int end, idk::vector<T> &path);
};

#undef adjlist_t




template <typename T>
void
idk::Graph<T>::addVertex( int v, const T &data )
{
    m_vertices[v] = data;
}


template <typename T>
const T&
idk::Graph<T>::getVertex( int v )
{
    return m_vertices[v];
}


template <typename T>
void
idk::Graph<T>::addEdge( int v0, int v1, float weight )
{
    m_edges[v0][v1] = weight;
}


template <typename T>
void
idk::Graph<T>::removeEdge( int v0, int v1 )
{
    m_edges[v0][v1] = std::numeric_limits<float>::infinity();
}


template <typename T>
void
idk::Graph<T>::shortestPath( int start, int end, idk::vector<T> &path )
{
    std::set<std::pair<float, int>> pq;
    idk::vector<float> dist(m_vertices.size(), std::numeric_limits<float>::infinity());
    idk::vector<int> prev(m_vertices.size(), -1);

    dist[start] = 0.0f;
    pq.insert({0.0f, start});

    while (pq.empty() == false)
    {
        int u = pq.begin()->second;
        pq.erase(pq.begin());

        for (auto &p: m_edges[u])
        {
            int v = p.first;
            float weight = p.second;

            if (dist[v] > dist[u] + weight)
            {
                pq.erase({dist[v], v});
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.insert({dist[v], v});
            }
        }
    }

    path.resize(0);

    int idx = end;
    while (idx != -1)
    {
        path.push(m_vertices[idx]);
        idx = prev[idx];
    }
}

