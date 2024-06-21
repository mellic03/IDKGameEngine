#pragma once

#include "idk_ecs2.hpp"






template <typename T>
void
idk::ECS2::giveComponent( int obj_id )
{
    size_t key = getkey<ComponentArray<T>>();
    giveComponent(obj_id, key);
}


template <typename T>
bool
idk::ECS2::hasComponent( int obj_id )
{
    size_t key = getkey<ComponentArray<T>>();
    return hasComponent(obj_id, key);
}


template <typename T>
T&
idk::ECS2::getComponent( int obj_id )
{
    std::string msg = "Object " + std::to_string(obj_id) + " does not exist";
    IDK_ASSERT(msg.c_str(), m_entities.contains(obj_id));

    msg = "Object " + std::to_string(obj_id) + " does not have component ";
    msg += getComponentArray<T>().getName();
    IDK_ASSERT(msg.c_str(), hasComponent<T>(obj_id));

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
    return *dynamic_cast<ComponentArray<T> *>(m_component_arrays[key].get());
}





template <typename T>
void
idk::ECS2::registerComponent( const std::string &name )
{
    size_t key = getkey<ComponentArray<T>>();
 
    m_component_arrays[key] = std::make_unique<ComponentArray<T>>(name);
    m_component_keys[name] = key;
}


