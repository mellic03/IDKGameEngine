#include "idk_ecs2.hpp"



int
idk::ECS2::createGameObject( const std::string &name )
{
    Entity e;
    e.name = name;
    return m_entities.create(e);
}



void
idk::ECS2::deleteGameObject( int obj_id, bool deep )
{
    auto &children = m_entities.get(obj_id).children;

    if (deep)
    {
        for (int child_id: children)
        {
            deleteGameObject(child_id, true);
        }
    }

    else
    {
        for (int child_id: children)
        {
            removeChild(obj_id, child_id);
        }
    }

    m_entities.destroy(obj_id);
}


int
idk::ECS2::getParent( int obj_id )
{
    return m_entities.get(obj_id).parent;
}


void
idk::ECS2::giveChild( int parent_id, int child_id )
{
    m_entities.get(parent_id).children.insert(child_id);
}


void
idk::ECS2::removeChild( int parent_id, int child_id )
{
    m_entities.get(parent_id).children.erase(child_id);
}



void
idk::ECS2::save( const std::string &filepath )
{
    std::ofstream stream(filepath, std::ios::binary);

    // Write list of component names
    // --------------------------------------------------
    std::vector<std::string> names;

    for (iComponentArray *C: m_component_arrays)
    {
        names.push_back(C->getName());
    }

    idk::streamwrite(stream, names);
    // --------------------------------------------------


    // Write components
    // --------------------------------------------------
    for (iComponentArray *cmp: m_component_arrays)
    {
        cmp->serialize(stream);
    }
    // --------------------------------------------------


    // Write components
    // --------------------------------------------------
    m_entities.serialize(stream);
    // --------------------------------------------------


    stream.close();
}


void
idk::ECS2::load( const std::string &filepath )
{
    std::ifstream stream(filepath, std::ios::binary);

    stream.close();
}


