#include "idk_ecs2.hpp"
#include "IDKBuiltinCS/sys-transform.hpp"



void
idk::ECS2::init( idk::EngineAPI &api )
{
    m_api_ptr = &api;

    for (System *system: m_systems)
    {
        system->init(api);
    }
}


void
idk::ECS2::update( idk::EngineAPI &api )
{
    for (System *system: m_systems)
    {
        system->update(api);
    }

    if (m_readfile)
    {
        m_readfile = false;
        _load();
    }
}


void
idk::ECS2::shutdown( idk::EngineAPI &api )
{
    for (System *system: m_systems)
    {
        system->shutdown(api);
    }

    LOG_INFO() << "idk::ECS2::shutdown";
}




void
idk::ECS2::removeComponent( int obj_id, size_t key )
{
    if (hasComponent(obj_id, key) == false)
    {
        return;
    }

    auto &e = m_entities.get(obj_id);
    auto *c = getComponentArray(key);

    if (e.components.contains(key) == false)
    {
        LOG_WARN() << "Object " << obj_id << " does not have component " << key;
    }

    c->onObjectDeassignment(*m_api_ptr, obj_id);
    c->destroyComponent(e.components[key]);

    e.components.erase(key);
}




int
idk::ECS2::createGameObject( const std::string &name, bool persistent )
{
    int id = m_entities.create();
    Entity &e = m_entities.get(id);
    e.id = id;
    e.persistent = persistent;
    e.name = name;

    giveComponent<IconCmp>(id);
    giveComponent<TransformCmp>(id);

    return id;
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
        getComponentArray(key)->onObjectCopy(obj_id, new_obj);
    }

    return new_obj;
}


void
idk::ECS2::deleteGameObject( int obj_id, bool deep )
{
    // Delete children
    // --------------------------------------------
    std::vector<int> cull;

    for (int child_id: m_entities.get(obj_id).children)
    {
        cull.push_back(child_id);
    }

    for (int child_id: cull)
    {
        removeChild(obj_id, child_id);
        deleteGameObject(child_id, true);
    }
    // --------------------------------------------

 

    std::vector<size_t> keys;

    for (auto &[key, cmp]: m_entities.get(obj_id).components)
    {
        keys.push_back(key);
    }

    for (size_t key: keys)
    {
        removeComponent(obj_id, key);
    }

    m_entities.destroy(obj_id);

}


void
idk::ECS2::gameObjectPersistent( int obj_id, bool p )
{
    m_entities.get(obj_id).persistent = p;
}


bool*
idk::ECS2::getGameObjectPersistency( int obj_id )
{
    return &(m_entities.get(obj_id).persistent);
}




bool
idk::ECS2::gameObjectExists( int obj_id )
{
    return m_entities.contains(obj_id);
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
    LOG_INFO() << "ECS2::copySelectedGameObject";
    IDK_ASSERT("No selected object!", m_selected_object != -1);
    return copyGameObject(m_selected_object);
}


const std::string &
idk::ECS2::getSelectedGameObjectName()
{
    IDK_ASSERT("No selected object!", m_selected_object != -1);
    return getGameObjectName(m_selected_object);
}







void
idk::ECS2::giveComponent( int obj_id, size_t key )
{
    auto &e = m_entities.get(obj_id);
    auto &c = m_component_arrays[key];
    e.components[key] = c->createComponent(obj_id);
    c->onObjectAssignment(*m_api_ptr, obj_id);
}


bool
idk::ECS2::hasComponent( int obj_id, size_t key )
{
    IDK_ASSERT("Object does not exist", m_entities.contains(obj_id));

    auto &e = m_entities.get(obj_id);
    return e.components.contains(key);
}


int
idk::ECS2::getParent( int obj_id )
{
    // IDK_ASSERT("Object does not have parent", hasParent(obj_id));
    return m_entities.get(obj_id).parent;
}


bool
idk::ECS2::hasParent( int obj_id )
{
    return m_entities.get(obj_id).parent != -1;
}


const std::set<int>&
idk::ECS2::getChildren( int obj_id )
{
    return m_entities.get(obj_id).children;
}



void
idk::ECS2::giveChild( int parent_id, int child_id )
{
    glm::mat4 Mw = TransformSys::getWorldMatrix(child_id);
    glm::mat4 Ml = TransformSys::getLocalMatrix(child_id, false);
    glm::mat4 R  = glm::mat4(glm::mat3(Mw*Ml));
    glm::vec3 child_pos = TransformSys::getPositionWorldspace(child_id);

    removeChild(getParent(child_id), child_id);

    m_entities.get(parent_id).children.insert(child_id);
    m_entities.get(child_id).parent = parent_id;

    if (m_readfile == false)
    {
        TransformSys::recomputeTransformMatrices(child_id);
        TransformSys::setPositionWorldspace(child_id, child_pos);

        Mw = TransformSys::getWorldMatrix(child_id);
        Ml = TransformSys::getLocalMatrix(child_id, false);
        R  = glm::inverse(Mw*Ml) * R;
        glm::quat Q = glm::normalize(glm::quat_cast(R));
        TransformSys::getData(child_id).rotation = Q;
    }
}


void
idk::ECS2::removeChild( int parent_id, int child_id )
{
    // glm::mat4 Mw = TransformSys::getWorldMatrix(child_id);
    // glm::mat4 Ml = TransformSys::getLocalMatrix(child_id, false);
    // glm::mat4 R  = glm::mat4(glm::mat3(Mw*Ml));
    // glm::vec3 child_pos = TransformSys::getPositionWorldspace(child_id);

    if (parent_id != -1)
    {
        m_entities.get(parent_id).children.erase(child_id);
        m_entities.get(child_id).parent = -1;
    }

    // if (m_readfile == false)
    // {
    //     TransformSys::recomputeTransformMatrices(child_id);
    //     TransformSys::setPositionWorldspace(child_id, child_pos);

    //     Mw = TransformSys::getWorldMatrix(child_id);
    //     Ml = TransformSys::getLocalMatrix(child_id, false);
    //     R  = glm::inverse(Mw*Ml) * R;
    //     glm::quat Q = glm::normalize(glm::quat_cast(R));
    //     TransformSys::getData(child_id).rotation = Q;
    // }
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
idk::ECS2::_load()
{
    std::ifstream stream(m_filepath, std::ios::binary);

    // Read list of component names
    // --------------------------------------------------
    std::vector<std::string> names;
    idk::streamread(stream, names);
    // --------------------------------------------------

    // Read components
    // --------------------------------------------------
    std::cout << "Reading components -------------------\n";
    for (std::string name: names)
    {
        auto *C = getComponentArray(name);

        if (C)
        {
            std::cout << "Reading component array: " << name << "\n";
            C->deserialize(stream);
        }
    }
    // --------------------------------------------------

    // Read entities
    // --------------------------------------------------
    std::cout << "Reading entities ---------------------\n";
    m_entities.deserialize(stream);
    // --------------------------------------------------

    stream.close();

    init(*m_api_ptr);

    // Absolutely terrible way to handle persistency.
    // --------------------------------------------------
    // std::vector<int> cull;

    // for (Entity &e: m_entities)
    // {
    //     if (e.persistent == false)
    //     {
    //         cull.push_back(e.id);
    //     }
    // }

    // for (int id: cull)
    // {
    //     std::cout << "Non-persistent object: " << id << "\n";
    //     deleteGameObject(id, true);
    // }
    // --------------------------------------------------

    update(*m_api_ptr);

}


void
idk::ECS2::load( const std::string &filepath )
{
    m_readfile = true;
    m_filepath = filepath;
}


