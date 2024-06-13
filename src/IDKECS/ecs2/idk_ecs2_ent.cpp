#include "idk_ecs2.hpp"



size_t
idk::ECS2::Entity::serialize( std::ofstream &stream )
{
    std::vector<std::string> names;
    std::vector<int>         cmp_ids;

    for (auto &[key, cmp_id]: components)
    {
        std::string name = ECS2::getComponentArray(key);
        names.push_back(name);
        cmp_ids.push_back(cmp_id);
    }

    size_t n = 0;
    n += idk::streamwrite(stream, name);
    n += idk::streamwrite(stream, parent);
    n += idk::streamwrite(stream, children);
    n += idk::streamwrite(stream, names);
    n += idk::streamwrite(stream, cmp_ids);
    return n;
}


size_t
idk::ECS2::Entity::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, name);
    n += idk::streamread(stream, parent);
    n += idk::streamread(stream, children);

    std::vector<std::string> names;
    std::vector<int>         cmp_ids;
    n += idk::streamread(stream, names);
    n += idk::streamread(stream, cmp_ids);

    for (int i=0; i<names.size(); i++)
    {
        size_t key = ECS2::getComponentKey(names[i]);
        components[key] = cmp_ids[i];
    }

    return n;
}