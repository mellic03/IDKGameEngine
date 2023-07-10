#pragma once

#include "idk_lightsource.h"

namespace idk
{
    class PointlightAllocator;
};



class idk::PointlightAllocator
{
private:
    int                     _max_size;
    int                     _num_active = 0;
    idk::stack<int>         _unused_ids;

public:
    std::vector<glm::vec4> position;
    std::vector<glm::vec4> ambient;
    std::vector<glm::vec4> diffuse;
    std::vector<glm::vec4> attenuation;
    std::vector<bool>       is_occupied;

    PointlightAllocator( int max_size ):
    position(max_size), ambient(max_size),
    diffuse(max_size),  attenuation(max_size),
    is_occupied(max_size), _max_size(max_size)
    {
        for (int i=0; i<max_size; i++)
        {
            is_occupied[i] = false;
            _unused_ids.push(i);
        }
    };

    int     add();
    void    remove( int pointlight_id );
    int     numActive() { return _num_active; };
};



