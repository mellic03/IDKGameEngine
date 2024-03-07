#include "IDKECS.hpp"

#include <fstream>
#include <libidk/idk_io.hpp>
#include <libidk/idk_view.hpp>

#include "idk_ecs_file.hpp"
#include "IDKBuiltinCS/sys-transform.hpp"


static idk::EngineAPI *api_ptr;
static bool            file_read_mode = false;

void
idk::ecs::ECS::init( idk::EngineAPI &api )
{
    api_ptr = &api;

    for (System *system: m_systems)
    {
        system->init(api);
    }
}


void
idk::ecs::ECS::update( idk::EngineAPI &api )
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
idk::ecs::ECS::writeEntities( std::ofstream &stream )
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

    idk::streamwrite(stream, entity_ids);
    idk::streamwrite(stream, hierarchy);
    idk::streamwrite(stream, m_entities);

}


void
idk::ecs::ECS::writeComponents( std::ofstream &stream )
{
    ECSFileHeader ecs_header = {
        .major = IDK_ECS_VERSION_MAJOR,
        .minor = IDK_ECS_VERSION_MINOR,
        .num_arrays = uint32_t(numComponents())
    };

    ECSFile_write(stream, ecs_header);

    for (auto &[key, cmp]: m_components)
    {
        ComponentFileHeader cmp_header = ComponentFile_new(cmp);
        ComponentFile_write(stream, cmp_header, cmp);
    }
}



void
idk::ecs::ECS::readEntities( std::ifstream &stream )
{
    m_entityIDs.clear();
    m_parent.clear();
    m_children.clear();

    std::vector<int> entity_ids;
    std::vector<int> hierarchy;

    idk::streamread(stream, entity_ids);
    idk::streamread(stream, hierarchy);
    idk::streamread(stream, m_entities);

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
idk::ecs::ECS::readComponents( std::ifstream &stream )
{
    ECSFileHeader ecs_header;
    ECSFile_read(stream, ecs_header);

    for (uint32_t i=0; i<ecs_header.num_arrays; i++)
    {
        ComponentFileHeader header;
        ComponentFile_readHeader(stream, header);

        if (componentExists(header.name))
        {
            std::cout << "reading \"" << header.name << "\" component\n";

            iComponentArray *comp_array = getComponentArray(header.name);
            ComponentFile_readData(stream, header, comp_array);
        }

        else
        {
            std::cout << "skipping " << header.name << " component\n";
            ComponentFile_skipData(stream, header);
        }
    }

}


void
idk::ecs::ECS::writeFile( const std::string &filepath )
{
    std::ofstream stream(filepath, std::ios::binary);

    writeComponents(stream);
    writeEntities(stream);

    stream.close();

    std::cout << "Scene saved to \"" << filepath << "\"\n";
}



void
idk::ecs::ECS::_readFile( const std::string &filepath )
{
    file_read_mode = true;

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

    getSystem<idk::TransformSys>().update(m_api);

    for (auto &[id, cmp]: m_components)
    {
        cmp->allObjectAssignment();
    }

    file_read_mode = false;
}


void
idk::ecs::ECS::readFile( const std::string &filepath )
{
    m_readFile_path   = filepath;
    m_should_readFile = true;
}



size_t
idk::ecs::ECS::getComponentKey( const std::string &name )
{
    return m_component_keys[name];
}


bool
idk::ecs::ECS::componentExists( const std::string &name )
{
    return m_component_keys.contains(name);
}


int
idk::ecs::ECS::getComponentID( size_t key )
{
    return m_component_IDs[key];
}



idk::ecs::iComponentArray *
idk::ecs::ECS::getComponentArray( const std::string &name )
{
    IDK_ASSERT("No such component array", m_component_keys.contains(name));

    size_t key = m_component_keys[name];
    int    id  = m_component_IDs[key];

    return getComponentArray(id);
}


void
idk::ecs::ECS::setSelectedGameObject( int obj_id )
{
    m_selection.push_back(obj_id);

    if (m_selection.size() > 10)
    {
        m_selection.pop_front();
    }
}


int
idk::ecs::ECS::getSelectedGameObject()
{
    if (m_selection.size() > 0)
    {
        return m_selection.back();
    }

    return -1;
}


void
idk::ecs::ECS::deleteSelectedGameObject()
{
    int obj_id = getSelectedGameObject();

    if (obj_id != -1)
    {
        deleteGameObject(obj_id);
        m_selection.pop_back();
    }
}


int
idk::ecs::ECS::copySelectedGameObject()
{
    int obj_id = getSelectedGameObject();

    if (obj_id != -1)
    {
        return copyGameObject(obj_id);
    }

    return -1;
}



const std::string &
idk::ecs::ECS::getSelectedGameObjectName()
{
    int obj_id = getSelectedGameObject();

    IDK_ASSERT(
        "Called idk::ecs::ECS::getSelectedGameObjectName with no selected object",
        obj_id != -1
    );

    return getGameObjectName(obj_id);
}



void
idk::ecs::ECS::giveComponent( int obj_id, int component )
{
    if (hasComponent(obj_id, component))
    {
        return;
    }

    Entity &entity  = m_entities.get(obj_id);
    int     data_id = getComponentArray(component)->create(obj_id);

    entity.components[component] = data_id;
}


void
idk::ecs::ECS::copyComponent( int dst_id, int src_id, int component )
{
    Entity &src = m_entities.get(src_id);
    Entity &dst = m_entities.get(dst_id);

    getComponentArray(component)->copy(
        src_id,
        dst_id,
        dst.components[component],
        src.components[component]
    );
}


bool
idk::ecs::ECS::hasComponent( int obj_id, int component )
{
    if (obj_id < 0 || component < 0)
    {
        return false;
    }

    Entity &entity = m_entities.get(obj_id);

    IDK_ASSERT(
        "Index out of range",
        component < entity.components.size()
    );

    return entity.components[component] != -1;
}


void
idk::ecs::ECS::removeComponent( int obj_id, int component_id )
{
    Entity &entity  = m_entities.get(obj_id);
    int    &data_id = entity.components[component_id];

    getComponentArray(component_id)->destroy(obj_id, data_id);

    data_id = -1;
}





int
idk::ecs::ECS::createGameObject( const std::string &name )
{
    int id = m_entities.create(idk::ecs::createEntity(name));

    m_entityIDs.insert(id);

    for (int component: m_mandatory_components)
    {
        giveComponent(id, component);
    }

    m_parent[id] = -1;
    return id;
}



int
idk::ecs::ECS::copyGameObject( int src_id )
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
        }
    }

    return dst_id;
}


void
idk::ecs::ECS::deleteGameObject( int obj_id )
{
    auto &children = getChildren(obj_id);
    int  parent_id = m_parent[obj_id];

    file_read_mode = true;

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

    file_read_mode = false;


    m_entities.destroy(obj_id);
    m_entityIDs.erase(obj_id);
}


std::set<int> &
idk::ecs::ECS::gameObjects()
{
    return m_entityIDs;
}


bool
idk::ecs::ECS::gameObjectExists( int obj_id )
{
    return m_entityIDs.contains(obj_id);
}



const std::string &
idk::ecs::ECS::getGameObjectName( int obj_id )
{
    return m_entities.get(obj_id).name;
}


void
idk::ecs::ECS::setGameObjectName( int obj_id, const std::string &name)
{
    getEntity(obj_id).name = name;
}


int
idk::ecs::ECS::getGameObjectByName( const std::string &name )
{
    for (int obj_id: gameObjects())
    {
        if (getGameObjectName(obj_id) == name)
        {
            return obj_id;
        }
    }

    return -1;
}



void
idk::ecs::ECS::giveChild( int parent_id, int child_id )
{
    glm::vec3 child_pos = TransformSys::getPositionWorldspace(child_id);

    removeChild(getParent(child_id), child_id);

    m_children[parent_id].insert(child_id);
    m_parent[child_id] = parent_id;

    if (file_read_mode == false)
    {
        TransformSys::recomputeTransformMatrices(child_id);
        TransformSys::setPositionWorldspace(child_id, child_pos);
    }
}


void
idk::ecs::ECS::removeChild( int parent_id, int child_id )
{
    glm::vec3 child_pos = TransformSys::getPositionWorldspace(child_id);

    m_children[parent_id].erase(child_id);
    m_parent[child_id] = -1;

    if (file_read_mode == false)
    {
        TransformSys::recomputeTransformMatrices(child_id);
        TransformSys::setPositionWorldspace(child_id, child_pos);
    }
}


void
idk::ecs::ECS::clearChildren( int parent_id )
{
    for (int child_id: m_children[parent_id])
    {
        removeChild(parent_id, child_id);
    }

    m_children[parent_id].clear();
}


void
idk::ecs::ECS::removeParent( int child_id )
{
    int current_parent = m_parent[child_id];
    if (current_parent >= 0)
    {
        removeChild(current_parent, child_id);
    }

    m_parent[child_id] = -1;
}



void
idk::ecs::ECS::exposeToLua( lua_State *L )
{
    luaaa::LuaModule mod(L, "ECS");

    mod.fun("loadScene", [this](std::string filepath)
    {
        readFile(filepath);
    });

    mod.fun("giveChild", [this](int parent, int child) { giveChild(parent, child); } );
    mod.fun("removeParent", [this](int child) { removeParent(child); } );
}
