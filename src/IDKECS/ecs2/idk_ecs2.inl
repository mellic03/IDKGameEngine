// #pragma once

// #include "idk_ecs2.hpp"






// template <typename T>
// void
// idk::ECS2::giveComponent( int obj_id )
// {
//     size_t key = getkey<ComponentArray<T>>();
//     giveComponent(obj_id, key);
// }



// // template <typename T, typename... Args>
// // void
// // idk::ECS2::giveComponents( int obj_id )
// // {
// //     giveComponent<T>(obj_id);
// //     giveComponents<Args...>(obj_id);
// // }



// template <typename T>
// bool
// idk::ECS2::hasComponent( int obj_id )
// {
//     size_t key = getkey<ComponentArray<T>>();
//     return hasComponent(obj_id, key);
// }


// template <typename T>
// T&
// idk::ECS2::getComponent( int obj_id )
// {
//     std::string msg = "Object " + std::to_string(obj_id) + " does not exist";
//     IDK_ASSERT(msg.c_str(), m_entities.contains(obj_id));

//     msg = "Object " + std::to_string(obj_id) + " does not have component ";
//     msg += getComponentArray<T>().getName();
//     IDK_ASSERT(msg.c_str(), hasComponent<T>(obj_id));

//     size_t key = getkey<ComponentArray<T>>();
//     auto   &e  = m_entities.get(obj_id);
//     auto   &c  = getComponentArray<T>();

//     return c.getComponent(e.components[key]);
// }



// template <typename T>
// void
// idk::ECS2::copyComponent( int src_obj, int dst_obj )
// {
//     auto &src = getComponent<T>(src_obj);
//     auto &dst = getComponent<T>(dst_obj);

//     dst = src;
//     dst.obj_id = dst_obj;
// }




// template <typename T>
// idk::ECS2::ComponentArray<T>&
// idk::ECS2::getComponentArray()
// {
//     size_t key = getkey<ComponentArray<T>>();
//     return *dynamic_cast<ComponentArray<T> *>(m_component_arrays[key].get());
// }





// template <typename T>
// void
// idk::ECS2::registerComponent( const std::string &name, const std::string &category )
// {
//     size_t key = getkey<ComponentArray<T>>();
 
//     m_component_arrays[key] = std::make_unique<ComponentArray<T>>(name, key);
//     m_component_keys[name] = key;

//     m_component_categories[category].push_back(key);
// }


