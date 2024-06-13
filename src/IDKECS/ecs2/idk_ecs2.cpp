#include "idk_ecs2.hpp"



int
idk::ECS2::createGameObject( const std::string &name )
{
    Entity e;
    e.name = name;
    return m_entities.create(e);
}


int
idk::ECS2::copyGameObject( int obj_id, bool deep )
{
    int new_obj = createGameObject();

    auto &src = m_entities.get(obj_id);
    auto &dst = m_entities.get(new_obj);

    dst.name = src.name;
    dst.parent = src.parent;
    dst.component_names = src.component_names;

    for (auto &[key, cmp]: src.components)
    {
        giveComponent(new_obj, key);
    }

    return new_obj;
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



const std::string &
idk::ECS2::getGameObjectName( int obj_id )
{
    return m_entities.get(obj_id).name;
}


void
idk::ECS2::setGameObjectName( int obj_id, const std::string &name)
{
    m_entities.get(obj_id).name = name;
}


int
idk::ECS2::getSelectedGameObject()
{
    return m_selected_object;
}


void
idk::ECS2::setSelectedGameObject( int obj_id )
{
    m_selected_object = obj_id;
}


void
idk::ECS2::deleteSelectedGameObject()
{
    if (m_selected_object != -1)
    {
        deleteGameObject(m_selected_object);
        m_selected_object = -1;
    }
}


int
idk::ECS2::copySelectedGameObject()
{
    if (m_selected_object != -1)
    {
        return copyGameObject(m_selected_object);
    }
}


const std::string &
idk::ECS2::getSelectedGameObjectName()
{
    if (m_selected_object != -1)
    {
        return getGameObjectName(m_selected_object);
    }
}







void
idk::ECS2::giveComponent( int obj_id, size_t key )
{
    auto &e = m_entities.get(obj_id);
    // auto &c = getComponentArray(key);
    e.components[key] = m_component_arrays[key]->createComponent();
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

    for (auto &[key, C]: m_component_arrays)
    {
        names.push_back(C->getName());
    }

    idk::streamwrite(stream, names);
    // --------------------------------------------------


    // Write components
    // --------------------------------------------------
    for (auto &[key, C]: m_component_arrays)
    {
        C->serialize(stream);
    }
    // --------------------------------------------------


    // Write entities
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


