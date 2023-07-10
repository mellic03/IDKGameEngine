#include "idk_lightsource_allocator.h"



int
idk::PointlightAllocator::add()
{
    if (_unused_ids.empty() == false)
    {
        int id = _unused_ids.pop();
        position[id] = glm::vec4(0.0f);
        ambient[id] = glm::vec4(0.1f);
        diffuse[id] = glm::vec4(155.0f);
        attenuation[id] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    
        is_occupied[id] = true;
        _num_active += 1;
        return id;
    }
    
    else
    {
        int id = _num_active;
        position[id] = glm::vec4(0.0f);
        ambient[id] = glm::vec4(0.1f);
        diffuse[id] = glm::vec4(155.0f);
        attenuation[id] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        is_occupied[id] = true;
    
        _num_active += 1;

        return id;
    }
}


void
idk::PointlightAllocator::remove( int pointlight_id )
{
    _unused_ids.push(pointlight_id);
    is_occupied[pointlight_id] = false;
    _num_active -= 1;
}