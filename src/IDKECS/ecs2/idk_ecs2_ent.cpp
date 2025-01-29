// #include "idk_ecs2.hpp"


// template<typename key_type, typename value_type>
// size_t
// idk_stdmap_streamwrite( std::ofstream &stream, const std::map<key_type, value_type> &data )
// {
//     size_t n = 0;

//     uint32_t size = uint32_t(data.size());
//     n += idk::streamwrite(stream, size);

//     for (const auto &[key, value]: data)
//     {
//         n += idk::streamwrite(stream, key);
//         n += idk::streamwrite(stream, value);
//     }

//     return n;
// }

// template<typename key_type, typename value_type>
// size_t
// idk_stdmap_streamread( std::ifstream &stream, std::map<key_type, value_type> &data )
// {
//     size_t n = 0;

//     uint32_t size;
//     n += idk::streamread(stream, size);

//     for (uint32_t i=0; i<size; i++)
//     {
//         key_type   key;
//         value_type value;

//         n += idk::streamread(stream, key);
//         n += idk::streamread(stream, value);

//         data[key] = value;
//     }

//     return n;
// }



// template<typename T>
// size_t
// idk_stdset_streamwrite( std::ofstream &stream, const std::set<T> &data )
// {
//     size_t n = 0;

//     uint32_t size = uint32_t(data.size());
//     n += idk::streamwrite(stream, size);

//     for (const auto &value: data)
//     {
//         n += idk::streamwrite(stream, value);
//     }

//     return n;
// }

// template<typename T>
// size_t
// idk_stdset_streamread( std::ifstream &stream, std::set<T> &data )
// {
//     size_t n = 0;

//     uint32_t size;
//     n += idk::streamread(stream, size);

//     for (uint32_t i=0; i<size; i++)
//     {
//         T value;
//         n += idk::streamread(stream, value);
//         data.insert(value);
//     }

//     return n;
// }



// size_t
// idk::ECS2::Entity::serialize( std::ofstream &stream ) const
// {
//     std::vector<int> child_ids;

//     for (int child_id: children)
//     {
//         child_ids.push_back(child_id);
//     }

//     size_t n = 0;
//     n += idk::streamwrite(stream, id);
//     n += idk::streamwrite(stream, persistent);
//     n += idk::streamwrite(stream, name);
//     n += idk::streamwrite(stream, parent);
//     n += idk::streamwrite(stream, child_ids);
//     n += idk_stdset_streamwrite(stream, this->component_names);
//     n += idk_stdmap_streamwrite(stream, this->components);
//     return n;
// }


// size_t
// idk::ECS2::Entity::deserialize( std::ifstream &stream )
// {
//     size_t n = 0;
//     n += idk::streamread(stream, id);
//     n += idk::streamread(stream, persistent);
//     n += idk::streamread(stream, name);
//     n += idk::streamread(stream, parent);

//     std::vector<int> child_ids;
//     n += idk::streamread(stream, child_ids);

//     for (int child_id: child_ids)
//     {
//         children.insert(child_id);
//     }

//     n += idk_stdset_streamread(stream, this->component_names);
//     n += idk_stdmap_streamread(stream, this->components);

//     return n;
// }