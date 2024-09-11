#include "idk_ecs2.hpp"
#include "IDKBuiltinCS/sys-transform.hpp"

namespace
{
    std::map<std::string, std::function<int()>> m_prefabs;
}




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
    // const double A = 1023.0 / 1024.0;
    // const double B = 1.0 / 1024.0;

    // static int count = 0;
    // count += 1;

    for (System *system: m_systems)
    {
        // auto start = std::chrono::high_resolution_clock::now();

        system->update(api);

        // auto finish  = std::chrono::high_resolution_clock::now();
        // auto elapsed = std::chrono::duration<double>(finish-start).count();

        // system->m_avg_time = A*system->m_avg_time + B*static_cast<double>(elapsed);
    }

    // std::cout << "Objects: ";

    // for (Entity &e: m_entities)
    // {
    //     std::cout << e.id << " ";
    // }
    // std::cout << std::endl;


    // if (count == 1024)
    // {
    //     count = 0;

    //     // for (System *system: m_systems)
    //     // {
    //     //     LOG_INFO()
    //     //         << "[ECS2::update] Avg. execution " << system->m_avg_time
    //     //         << " ms --> \"" << system->m_name << "\"";
    //     // }

    //     // LOG_INFO() << "";
    // }

    if (m_readfile)
    {
        m_readfile = false;
        _load();
    }


    for (int obj_id: m_delete_list)
    {
        _deleteGameObject(obj_id, true);
    }
    m_delete_list.clear();

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



bool
idk::ECS2::isRegisteredComponent( const std::string &name )
{
    for (auto &[key, ptr]: m_component_arrays)
    {
        if ((ptr.get())->getName() == name)
        {
            return true;
        }
    }

    return false;
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



idk::ECS2::iComponentArray *
idk::ECS2::getComponentArray( size_t key )
{
    return m_component_arrays[key].get();
}


idk::ECS2::iComponentArray *
idk::ECS2::getComponentArray( const std::string &name )
{
    size_t key = m_component_keys[name];
    return getComponentArray(key);
}


std::map<size_t, std::unique_ptr<idk::ECS2::iComponentArray>> &
idk::ECS2::getComponentArrays()
{
    return m_component_arrays;
}


std::map<std::string, idk::ECS2::iComponentArray*>
idk::ECS2::getComponentArraysSorted()
{
    std::map<std::string, iComponentArray*> arrays;

    for (auto &[key, ptr]: m_component_arrays)
    {
        arrays[ptr.get()->getName()] = ptr.get();
    }

    return arrays;
}



std::map<std::string, idk::ECS2::iComponentArray*>
idk::ECS2::getComponentArraysByCategory( const std::string &category )
{
    std::map<std::string, iComponentArray*> arrays;

    for (size_t key: m_component_categories[category])
    {
        iComponentArray *C = getComponentArray(key);
        arrays[C->getName()] = C;
    }

    return arrays;
}








int
idk::ECS2::createGameObject( const std::string &name, bool persistent )
{
    int id = m_entities.create();
    std::cout << "[ECS2::createGameObject] obj_id " << id << "\n";

    Entity &e = m_entities.get(id);
    e.id = id;
    e.persistent = persistent;
    e.name = name;
    e.children.clear();
    e.components.clear();
    e.component_names.clear();
    e.parent = -1;

    giveComponent<IconCmp>(id);
    giveComponent<TransformCmp>(id);

    return id;
}


int
idk::ECS2::copyGameObject( int src_obj, bool deep )
{
    int dst_obj = createGameObject();

    auto &src = m_entities.get(src_obj);
    auto &dst = m_entities.get(dst_obj);

    dst.id     = dst_obj;
    dst.name   = src.name;
    dst.persistent = src.persistent;

    for (auto &[key, cmp]: src.components)
    {
        giveComponent(dst_obj, key);
        getComponentArray(key)->onObjectCopy(src_obj, dst_obj);
    }

    for (int src_child: src.children)
    {
        int dst_child = copyGameObject(src_child, true);
        giveChild(dst_obj, dst_child);
    }

    if (hasParent(src_obj))
    {
        giveChild(getParent(src_obj), dst_obj);
    }

    return dst_obj;
}


void
idk::ECS2::_deleteGameObject( int obj_id, bool deep )
{
    LOG_DEBUG() << "[ECS2::deleteGameObject] obj_id " << obj_id;

    // Delete children
    // --------------------------------------------
    std::vector<int> cull;

    for (int child_id: m_entities.get(obj_id).children)
    {
        cull.push_back(child_id);
    }

    for (int child_id: cull)
    {
        _deleteGameObject(child_id, true);
    }

    int parent = getParent(obj_id);

    if (parent != -1)
    {
        removeChild(parent, obj_id);
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
idk::ECS2::deleteGameObject( int obj_id, bool deep )
{
    LOG_DEBUG() << "[ECS2::deleteGameObject] obj_id " << obj_id;

    m_delete_list.push_back(obj_id);
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
    if (hasComponent(obj_id, key))
    {
        LOG_WARN() << "Object " << obj_id << " already has component " << getComponentArray(key)->getName();
        return;
    }

    auto &e = m_entities.get(obj_id);
    auto *C = m_component_arrays[key].get();
    e.components[key] = C->createComponent(obj_id);
    C->onObjectAssignment(*m_api_ptr, obj_id);
}


bool
idk::ECS2::hasComponent( int obj_id, size_t key )
{
    std::string msg = "Object " + std::to_string(obj_id) + " does not exist";
    IDK_ASSERT(msg.c_str(), m_entities.contains(obj_id));

    auto &e = m_entities.get(obj_id);
    return e.components.contains(key);
}


int
idk::ECS2::getParent( int obj_id )
{
    // IDK_ASSERT("Object does not have parent", hasParent(obj_id));
    return m_entities.get(obj_id).parent;
}


const std::set<int>&
idk::ECS2::getChildren( int obj_id )
{
    std::string msg = "Object " + std::to_string(obj_id) + " does not exist";
    IDK_ASSERT(msg.c_str(), m_entities.contains(obj_id));

    return m_entities.get(obj_id).children;
}



bool
idk::ECS2::hasParent( int obj_id )
{
    std::string msg = "Object " + std::to_string(obj_id) + " does not exist";
    IDK_ASSERT(msg.c_str(), m_entities.contains(obj_id));

    return m_entities.get(obj_id).parent != -1;
}


bool
idk::ECS2::hasChildren( int obj_id )
{
    std::string msg = "Object " + std::to_string(obj_id) + " does not exist";
    IDK_ASSERT(msg.c_str(), m_entities.contains(obj_id));

    return m_entities.get(obj_id).children.size() > 0;
}


void
idk::ECS2::giveChild( int parent_id, int child_id )
{
    // glm::mat4 Mw = TransformSys::getWorldMatrix(child_id);
    // glm::mat4 Ml = TransformSys::getLocalMatrix(child_id, false);
    // glm::mat4 R  = glm::mat4(glm::mat3(Mw*Ml));
    // glm::vec3 child_pos = TransformSys::getPositionWorldspace(child_id);
    glm::vec3 child_pos = TransformSys::getWorldPosition(child_id);

    removeChild(getParent(child_id), child_id);

    m_entities.get(parent_id).children.insert(child_id);
    m_entities.get(child_id).parent = parent_id;

    if (m_readfile == false)
    {
        TransformSys::recomputeTransformMatrices(child_id);
        TransformSys::setWorldPosition(child_id, child_pos);

        // Mw = TransformSys::getWorldMatrix(child_id);
        // Ml = TransformSys::getLocalMatrix(child_id, false);
        // R  = glm::inverse(Mw*Ml) * R;
        // glm::quat Q = glm::normalize(glm::quat_cast(R));
        // TransformSys::getLocalRotation(child_id) = Q;
    }
}


void
idk::ECS2::removeChild( int parent_id, int child_id )
{
    // glm::mat4 Mw = TransformSys::getWorldMatrix(child_id);
    // glm::mat4 Ml = TransformSys::getLocalMatrix(child_id, false);
    // glm::mat4 R  = glm::mat4(glm::mat3(Mw*Ml));
    // glm::vec3 child_pos = TransformSys::getPositionWorldspace(child_id);
    glm::vec3 child_pos = TransformSys::getWorldPosition(child_id);

    if (parent_id != -1)
    {
        m_entities.get(parent_id).children.erase(child_id);
        m_entities.get(child_id).parent = -1;
    }

    if (m_readfile == false)
    {
        TransformSys::recomputeTransformMatrices(child_id);
        TransformSys::setWorldPosition(child_id, child_pos);

        // Mw = TransformSys::getWorldMatrix(child_id);
        // Ml = TransformSys::getLocalMatrix(child_id, false);
        // R  = glm::inverse(glm::mat4(glm::mat3(Mw*Ml))) * R;
        // glm::quat Q = glm::normalize(glm::quat_cast(R));

        // TransformSys::getLocalRotation(child_id) = Q;
    }
}




void
idk::ECS2::registerPrefab( const std::string &name, std::function<int()> ctor )
{
    m_prefabs[name] = ctor;
}

int
idk::ECS2::createGameObjectFromPrefab( const std::string &name )
{
    return m_prefabs[name]();
}

const std::map<std::string, std::function<int()>>&
idk::ECS2::getPrefabs()
{
    return m_prefabs;
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
        names.push_back((C.get())->getName());
    }

    idk::streamwrite(stream, names);
    // --------------------------------------------------

    // Write components
    // --------------------------------------------------
    for (auto &[key, C]: m_component_arrays)
    {
        (C.get())->serialize(stream);
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
    for (auto &callback: m_pre_callbacks)
    {
        callback();
    }


    ECS2::update(*m_api_ptr);

    for (System *S: m_systems)
    {
        S->shutdown(*m_api_ptr);
    }

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
        std::cout << "Reading component array: " << name << "\n";

        if (isRegisteredComponent(name) == false)
        {
            LOG_ERROR() << "Component \"" << name << "\" not registered, ignoring";
            continue;
        }

        auto *C = getComponentArray(name);

        if (C)
        {
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

    // init(*m_api_ptr);
    // update(*m_api_ptr);

    // Absolutely terrible way to handle persistency.
    // --------------------------------------------------
    std::vector<int> cull;

    for (Entity &e: m_entities)
    {
        if (e.persistent == false)
        {
            cull.push_back(e.id);
        }
    }

    for (int id: cull)
    {
        std::cout << "Non-persistent object: " << id << "\n";

        if (gameObjectExists(id))
        {
            _deleteGameObject(id, true);
        }
    }
    // --------------------------------------------------

    update(*m_api_ptr);

    for (auto &callback: m_callbacks)
    {
        callback();
    }
}


void
idk::ECS2::load( const std::string &filepath )
{
    m_readfile = true;
    m_filepath = filepath;
}


