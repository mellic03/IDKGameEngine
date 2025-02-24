#include "idk_ecs.hpp"
#include "IDKBuiltinCS/sys-transform.hpp"
#include <libidk/idk_log.hpp>

#include <filesystem>


namespace
{
    std::map<std::string, std::function<int()>> m_prefabs;
}



// void
// idk::ECS::init( idk::EngineAPI &api )
// {
//     m_api_ptr = &api;

//     for (System *system: m_systems)
//     {
//         system->init(api);
//     }
// }


void
idk::ECS::update( idk::EngineAPI &api )
{
    // if (m_api_ptr == nullptr)
    // {
    //     init(api);
    // }

    for (System *system: m_systems)
    {
        system->update(api);
    }

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
idk::ECS::shutdown( idk::EngineAPI &api )
{
    for (System *system: m_systems)
    {
        system->shutdown(api);
    }

    // LOG_INFO("idk::ECS::shutdown");
    LOG_INFO("");
}



bool
idk::ECS::isRegisteredComponent( const std::string &name )
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
idk::ECS::removeComponent( int obj_id, size_t key )
{
    if (hasComponent(obj_id, key) == false)
    {
        return;
    }

    auto &e = m_entities.get(obj_id);
    auto *c = getComponentArray(key);

    if (e.components.contains(key) == false)
    {
        LOG_WARN("Object {} does not have component {}", obj_id, key);
    }

    c->onObjectDeassignment(*m_api_ptr, obj_id);
    c->destroyComponent(e.components[key]);

    e.components.erase(key);
}



idk::ECS::iComponentArray *
idk::ECS::getComponentArray( size_t key )
{
    return m_component_arrays[key].get();
}


idk::ECS::iComponentArray *
idk::ECS::getComponentArray( const std::string &name )
{
    size_t key = m_component_keys[name];
    return getComponentArray(key);
}


std::map<size_t, std::unique_ptr<idk::ECS::iComponentArray>> &
idk::ECS::getComponentArrays()
{
    return m_component_arrays;
}


std::map<std::string, idk::ECS::iComponentArray*>
idk::ECS::getComponentArraysSorted()
{
    std::map<std::string, iComponentArray*> arrays;

    for (auto &[key, ptr]: m_component_arrays)
    {
        arrays[ptr.get()->getName()] = ptr.get();
    }

    return arrays;
}



std::map<std::string, idk::ECS::iComponentArray*>
idk::ECS::getComponentArraysByCategory( const std::string &category )
{
    std::map<std::string, iComponentArray*> arrays;

    for (size_t key: m_component_categories[category])
    {
        iComponentArray *C = getComponentArray(key);
        arrays[C->getName()] = C;
    }

    return arrays;
}



std::function<void(idk::EngineAPI&, idk::ECS&, int)>
idk::ECS::getUserCallback( size_t key )
{
    if (m_user_callbacks.contains(key) == false)
    {
        LOG_WARN("m_user_callbacks does not contain \"{}\"", key);

        return m_default_userfn;
    }
    return m_user_callbacks[key];
}





int
idk::ECS::createGameObject( const std::string &name, bool persistent )
{
    int id = m_entities.create();
    LOG_DEBUG("Created object with id {}", id);

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
idk::ECS::copyGameObject( int src_obj, bool deep )
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
idk::ECS::_deleteGameObject( int obj_id, bool deep )
{
    LOG_DEBUG("Deleted object {}", obj_id);

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
idk::ECS::deleteGameObject( int obj_id, bool deep )
{
    LOG_DEBUG("Scheduled deletion of object {}", obj_id);
    m_delete_list.push_back(obj_id);
}


void
idk::ECS::gameObjectPersistent( int obj_id, bool p )
{
    m_entities.get(obj_id).persistent = p;
}


bool*
idk::ECS::getGameObjectPersistency( int obj_id )
{
    return &(m_entities.get(obj_id).persistent);
}




bool
idk::ECS::gameObjectExists( int obj_id )
{
    return m_entities.contains(obj_id);
}



const std::string &
idk::ECS::getGameObjectName( int obj_id )
{
    return m_entities.get(obj_id).name;
}


void
idk::ECS::setGameObjectName( int obj_id, const std::string &name)
{
    m_entities.get(obj_id).name = name;
}


int
idk::ECS::getSelectedGameObject()
{
    return m_selected_object;
}


void
idk::ECS::setSelectedGameObject( int obj_id )
{
    m_selected_object = obj_id;
}


void
idk::ECS::deleteSelectedGameObject()
{
    if (m_selected_object != -1)
    {
        deleteGameObject(m_selected_object);
        m_selected_object = -1;
    }
}


int
idk::ECS::copySelectedGameObject()
{
    // LOG_INFO("idk::ECS::copySelectedGameObject");
    LOG_INFO("");
    IDK_ASSERT("No selected object!", m_selected_object != -1);
    return copyGameObject(m_selected_object);
}


const std::string &
idk::ECS::getSelectedGameObjectName()
{
    IDK_ASSERT("No selected object!", m_selected_object != -1);
    return getGameObjectName(m_selected_object);
}







void
idk::ECS::giveComponent( int obj_id, size_t key )
{
    if (hasComponent(obj_id, key))
    {
        LOG_WARN("Object {} already has component {}", obj_id, key);
        return;
    }

    auto &e = m_entities.get(obj_id);
    auto *C = m_component_arrays[key].get();
    e.components[key] = C->createComponent(obj_id);
    C->onObjectAssignment(*m_api_ptr, obj_id);
}


bool
idk::ECS::hasComponent( int obj_id, size_t key )
{
    std::string msg = "Object " + std::to_string(obj_id) + " does not exist";
    IDK_ASSERT(msg.c_str(), m_entities.contains(obj_id));

    auto &e = m_entities.get(obj_id);
    return e.components.contains(key);
}


int
idk::ECS::getParent( int obj_id )
{
    // IDK_ASSERT("Object does not have parent", hasParent(obj_id));
    return m_entities.get(obj_id).parent;
}


const std::set<int>&
idk::ECS::getChildren( int obj_id )
{
    std::string msg = "Object " + std::to_string(obj_id) + " does not exist";
    IDK_ASSERT(msg.c_str(), m_entities.contains(obj_id));

    return m_entities.get(obj_id).children;
}



bool
idk::ECS::hasParent( int obj_id )
{
    std::string msg = "Object " + std::to_string(obj_id) + " does not exist";
    IDK_ASSERT(msg.c_str(), m_entities.contains(obj_id));

    return m_entities.get(obj_id).parent != -1;
}


bool
idk::ECS::hasChildren( int obj_id )
{
    std::string msg = "Object " + std::to_string(obj_id) + " does not exist";
    IDK_ASSERT(msg.c_str(), m_entities.contains(obj_id));

    return m_entities.get(obj_id).children.size() > 0;
}


void
idk::ECS::giveChild( int parent_id, int child_id )
{
    // // glm::mat4 Mw = TransformSys::getWorldMatrix(child_id);
    // // glm::mat4 Ml = TransformSys::getLocalMatrix(child_id, false);
    // // glm::mat4 R  = glm::mat4(glm::mat3(Mw*Ml));
    // // glm::vec3 child_pos = TransformSys::getPositionWorldspace(child_id);
    // glm::vec3 child_pos = TransformSys::getWorldPosition(child_id);

    if (hasParent(child_id))
    {
        removeChild(getParent(child_id), child_id);
    }

    m_entities.get(parent_id).children.insert(child_id);
    m_entities.get(child_id).parent = parent_id;

    if (m_readfile == false)
    {
        // glm::mat4 M = TransformSys::getModelMatrix(parent_id);
        // TransformSys::recomputeTransformMatrices(child_id);
        // TransformSys::setWorldPosition(child_id, child_pos);

        // // Mw = TransformSys::getWorldMatrix(child_id);
        // // Ml = TransformSys::getLocalMatrix(child_id, false);
        // // R  = glm::inverse(Mw*Ml) * R;
        // // glm::quat Q = glm::normalize(glm::quat_cast(R));
        // // TransformSys::getLocalRotation(child_id) = Q;
    }
}


void
idk::ECS::removeChild( int parent_id, int child_id )
{
    // // glm::mat4 Mw = TransformSys::getWorldMatrix(child_id);
    // // glm::mat4 Ml = TransformSys::getLocalMatrix(child_id, false);
    // // glm::mat4 R  = glm::mat4(glm::mat3(Mw*Ml));
    // // glm::vec3 child_pos = TransformSys::getPositionWorldspace(child_id);
    // glm::vec3 child_pos = TransformSys::getWorldPosition(child_id);

    if (parent_id != -1)
    {
        m_entities.get(parent_id).children.erase(child_id);
        m_entities.get(child_id).parent = -1;
    }

    if (m_readfile == false)
    {
        // // int grandparent = getParent(parent_id);

        // // if (grandparent != -1)
        // {
        //     // glm::mat4 M = TransformSys::getModelMatrix(grandparent);
        //     TransformSys::recomputeTransformMatrices(parent_id);
        //     TransformSys::recomputeTransformMatrices(child_id);
        // }

        // TransformSys::setWorldPosition(child_id, child_pos);

        // // Mw = TransformSys::getWorldMatrix(child_id);
        // // Ml = TransformSys::getLocalMatrix(child_id, false);
        // // R  = glm::inverse(glm::mat4(glm::mat3(Mw*Ml))) * R;
        // // glm::quat Q = glm::normalize(glm::quat_cast(R));

        // // TransformSys::getLocalRotation(child_id) = Q;
    }
}




void
idk::ECS::registerPrefab( const std::string &name, const std::function<int()> &ctor )
{
    m_prefabs[name] = ctor;
}

int
idk::ECS::createGameObjectFromPrefab( const std::string &name )
{
    return m_prefabs[name]();
}

const std::map<std::string, std::function<int()>>&
idk::ECS::getPrefabs()
{
    return m_prefabs;
}




void
idk::ECS::save( const std::string &filepath )
{
    std::ofstream stream(filepath, std::ios::binary);

    // Write list of component names
    // --------------------------------------------------
    std::vector<std::string> names;
    std::vector<uint32_t>    sizes;

    for (auto &[key, C]: m_component_arrays)
    {
        names.push_back((C.get())->getName());
        sizes.push_back((C.get())->nbytes_serialized());
    }

    idk::streamwrite(stream, names);
    idk::streamwrite(stream, sizes);
    // --------------------------------------------------

    // Write components
    // --------------------------------------------------
    for (auto &[key, C]: m_component_arrays)
    {
        std::cout << "Writing component array: "
                  << (C.get())->getName()
                  << " (" << (C.get())->nbytes_serialized() << " bytes)\n";


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
idk::ECS::_load()
{
    m_pre_callback();

    for (System *S: m_systems)
    {
        S->shutdown(*m_api_ptr);
    }

    std::ifstream stream(m_filepath, std::ios::binary);

    // Read list of component names
    // --------------------------------------------------
    std::vector<std::string> names;
    std::vector<uint32_t>    sizes;
    idk::streamread(stream, names);
    idk::streamread(stream, sizes);
    // --------------------------------------------------

    // Read components
    // --------------------------------------------------
    std::cout << "Reading components -------------------\n";
    for (int i=0; i<names.size(); i++)
    {
        const auto &name = names[i];
        std::cout << "Reading component array: " << name << " (" << sizes[i] << " bytes)\n";

        if (isRegisteredComponent(name) == false)
        {
            std::cout << "Component \"" << name << "\" not registered, ignoring" << std::endl;
            stream.ignore(sizes[i]);
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

    // update(*m_api_ptr);

    m_post_callback();
}


void
idk::ECS::load( const std::string &filepath )
{
    std::filesystem::path fpath(filepath);
    if (std::filesystem::exists(fpath) == false)
    {
        LOG_ERROR("Cannot open file \"{}\"", filepath);
    }

    m_readfile = true;
    m_filepath = filepath;
}


