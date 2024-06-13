#pragma once

#include "idk_ecs2.hpp"






template <typename T>
void
idk::ECS2::giveComponent( int obj_id )
{
    size_t key = getkey<ComponentArray<T>>();

    auto &e = m_entities.get(obj_id);
    auto &c = getComponentArray<T>();

    e.components[key] = c.createComponent();
}


template <typename T>
bool
idk::ECS2::hasComponent( int obj_id )
{
    size_t key = getkey<ComponentArray<T>>();

    auto &e = m_entities.get(obj_id);
    return e.components.contains(key);
}



template <typename T>
T&
idk::ECS2::getComponent( int obj_id )
{
    if (hasComponent<T>(obj_id) == false)
    {
        giveComponent<T>(obj_id);
    }

    size_t key = getkey<ComponentArray<T>>();
    auto   &e  = m_entities.get(obj_id);
    auto   &c  = getComponentArray<T>();

    return c.getComponent(e.components[key]);
}



template <typename T>
idk::ECS2::ComponentArray<T>&
idk::ECS2::getComponentArray()
{
    size_t key = getkey<ComponentArray<T>>();

    iComponentArray *C = m_component_arrays[key];
    return *dynamic_cast<ComponentArray<T> *>(C);
}





template <typename T>
void
idk::ECS2::registerComponent( const std::string &name )
{
    size_t key = getkey<ComponentArray<T>>();
 
    m_component_arrays[key] = new ComponentArray<T>(name);
}

