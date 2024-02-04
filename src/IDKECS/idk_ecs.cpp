#include "IDKECS.hpp"

#include <fstream>
#include <libidk/idk_io.hpp>
#include <libidk/idk_view.hpp>


static idk::EngineAPI *api_ptr;

void
idecs::ECS::init( idk::EngineAPI &api )
{
    api_ptr = &api;

    for (System *system: m_systems)
    {
        system->init(api);
    }
}


void
idecs::ECS::update( idk::EngineAPI &api )
{
    for (System *system: m_systems)
    {
        system->update(api);
    }


    if (m_should_readFile)
    {
        m_should_readFile = false;
        _readFile(m_readFile_path);
    }

}



void
idecs::ECS::writeEntities( std::ofstream &stream )
{
    idk::vector<int> entity_ids;
    idk::vector<int> hierarchy;

    for (int id: m_entityIDs)
    {
        entity_ids.push_back(id);
    }

    for (int id: m_entityIDs)
    {
        hierarchy.push_back(id);
        hierarchy.push_back(getChildren(id).size());

        for (int child: getChildren(id))
        {
            hierarchy.push_back(child);
        }
    }

    stream << entity_ids;
    stream << hierarchy;
    stream << m_entities;
}


void
idecs::ECS::writeComponents( std::ofstream &stream )
{
    for (auto *cmp: m_components)
    {
        cmp->serialize(stream);
    }
}



void
idecs::ECS::readEntities( std::ifstream &stream )
{
    m_entityIDs.clear();
    m_parent.clear();
    m_children.clear();

    std::vector<int> entity_ids;
    std::vector<int> hierarchy;

    stream >> entity_ids;
    stream >> hierarchy;
    stream >> m_entities;

    for (int id: entity_ids)
    {
        m_entityIDs.insert(id);
        m_parent[id] = -1;
    }

    for (int i=0; i<hierarchy.size(); i++)
    {
        int id = hierarchy[i];
        int num_children = hierarchy[i+1];

        for (int child: idk::view(hierarchy[i+2], num_children))
        {
            giveChild(id, child);
        }

        i += num_children + 1;
    }

    for (int id: entity_ids)
    {
        auto &E = m_entities.get(id);

        for (int j=0; j<ECS::MAX_COMPONENTS; j++)
        {
            if (E.components[j] != -1)
            {
                giveComponent(id, j);
            }
        }
    }

}


void
idecs::ECS::readComponents( std::ifstream &stream )
{
    for (auto *cmp: m_components)
    {
        cmp->deserialize(stream);
    }
}


void
idecs::ECS::writeFile( const std::string &filepath )
{
    std::ofstream stream(filepath, std::ios::binary);

    writeComponents(stream);
    writeEntities(stream);

    stream.close();

    std::cout << "Scene saved to \"" << filepath << "\"\n";
}



void
idecs::ECS::_readFile( const std::string &filepath )
{
    std::ifstream stream(filepath, std::ios::binary);

    readComponents(stream);
    readEntities(stream);

    setSelectedGameObject(-1);

    for (auto *sys: m_systems)
    {
        sys->init(m_api);
    }

    stream.close();
    std::cout << "Scene read from \"" << filepath << "\"\n";
}


void
idecs::ECS::readFile( const std::string &filepath )
{
    m_readFile_path   = filepath;
    m_should_readFile = true;
}




void
idecs::ECS::setSelectedGameObject( int obj_id )
{
    m_selection.push_back(obj_id);

    if (m_selection.size() > 10)
    {
        m_selection.pop_front();
    }
}


int
idecs::ECS::getSelectedGameObject()
{
    if (m_selection.size() > 0)
    {
        return m_selection.back();
    }

    return -1;
}


void
idecs::ECS::deleteSelectedGameObject()
{
    int obj_id = getSelectedGameObject();

    if (obj_id != -1)
    {
        deleteGameObject(obj_id);
        m_selection.pop_back();
    }
}


int
idecs::ECS::copySelectedGameObject()
{
    int obj_id = getSelectedGameObject();

    if (obj_id != -1)
    {
        return copyGameObject(obj_id);
    }

    return -1;
}



char *
idecs::ECS::getSelectedGameObjectName()
{
    int obj_id = getSelectedGameObject();

    IDK_ASSERT(
        "Called idecs::ECS::getSelectedGameObjectName with no selected object",
        obj_id != -1
    );

    return getGameObjectName(obj_id);
}







void
idecs::ECS::giveComponent( int obj_id, int component )
{
    if (hasComponent(obj_id, component))
    {
        return;
    }

    Entity &entity  = m_entities.get(obj_id);
    int     data_id = getComponentArray(component)->create(obj_id);

    entity.components[component] = data_id;

    getComponentArray(component)->getBehaviour()._onAssignment(*api_ptr, obj_id);
}


void
idecs::ECS::copyComponent( int dst_id, int src_id, int component )
{
    Entity &src = m_entities.get(src_id);
    Entity &dst = m_entities.get(dst_id);

    getComponentArray(component)->copy(
        dst_id,
        dst.components[component],
        src.components[component]
    );
}


void
idecs::ECS::giveComponent( int obj_id, const std::string &name )
{
    IDK_ASSERT("Component does not exist", m_component_names.contains(name));
    giveComponent(obj_id, m_component_names[name]);
}



bool
idecs::ECS::hasComponent( int obj_id, int component_id )
{
    if (obj_id == -1)
    {
        return false;
    }

    if (component_id < 0 || component_id >= m_components.size())
    {
        return false;
    }

    Entity &entity = m_entities.get(obj_id);
    return entity.components[component_id] != -1;
}


void
idecs::ECS::removeComponent( int obj_id, int component_id )
{
    Entity &entity  = m_entities.get(obj_id);
    int    &data_id = entity.components[component_id];

    getComponentArray(component_id)->destroy(data_id);

    data_id = -1;
}







int
idecs::ECS::createGameObject( const std::string &name )
{
    int id = m_entities.create(idecs::createEntity(name));
    m_entityIDs.insert(id);

    for (int component: m_mandatory_components)
    {
        giveComponent(id, component);
    }

    m_parent[id] = -1;
    return id;
}



int
idecs::ECS::copyGameObject( int src_id )
{
    int dst_id = createGameObject(getGameObjectName(src_id));
    giveChild(getParent(src_id), dst_id);

    for (int child: getChildren(src_id))
    {
        giveChild(dst_id, copyGameObject(child));
    }

    for (int i=0; i<this->numComponents(); i++)
    {
        if (hasComponent(src_id, i))
        {
            giveComponent(dst_id, i);
            copyComponent(dst_id, src_id, i);

            // this->getComponentArray(i)->copy(dst_id, )

            // auto &B = this->getComponentArray(i)->getBehaviour();
            // B._onCopy(*api_ptr, src_id, dst_id);
        }
    }

    return dst_id;
}


void
idecs::ECS::deleteGameObject( int obj_id )
{
    auto &children = getChildren(obj_id);
    int  parent_id = m_parent[obj_id];

    for (int i=0; i<ECS::MAX_COMPONENTS; i++)
    {
        if (hasComponent(obj_id, i))
        {
            removeComponent(obj_id, i);
        }
    }


    if (parent_id >= 0)
    {
        for (int child_id: children)
        {
            giveChild(parent_id, child_id);
        }

        removeChild(parent_id, obj_id);
    }

    clearChildren(obj_id);

    m_entities.destroy(obj_id);
    m_entityIDs.erase(obj_id);
}


std::set<int> &
idecs::ECS::gameObjects()
{
    return m_entityIDs;
}


char *
idecs::ECS::getGameObjectName( int obj_id )
{
    char *name = &(m_entities.get(obj_id).name[0]);
    return name;
}


void
idecs::ECS::setGameObjectName( int obj_id, const std::string &name)
{
    std::strcpy(m_entities.get(obj_id).name, name.c_str());
}





void
idecs::ECS::giveChild( int parent_id, int child_id )
{
    int last_parent_id = m_parent[child_id];
    if (last_parent_id != -1)
    {
        m_children[last_parent_id].erase(child_id);
    }

    m_children[parent_id].insert(child_id);
    m_parent[child_id] = parent_id;
}


void
idecs::ECS::removeChild( int parent_id, int child_id )
{
    m_children[parent_id].erase(child_id);
    m_parent[child_id] = -1;
}


void
idecs::ECS::clearChildren( int parent_id )
{
    for (int child_id: m_children[parent_id])
    {
        m_parent[child_id] = -1;
    }

    m_children[parent_id].clear();
}


void
idecs::ECS::removeParent( int child_id )
{
    int current_parent = m_parent[child_id];
    if (current_parent >= 0)
    {
        removeChild(current_parent, child_id);
    }

    m_parent[child_id] = -1;
}



void
idecs::ECS::exposeToLua( lua_State *L )
{
    luaaa::LuaModule mod(L, "ECS");

    mod.fun("loadScene", [this](std::string filepath)
    {
        readFile(filepath);
    });
}
