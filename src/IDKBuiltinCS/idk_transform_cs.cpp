#include "idk_transform_cs.hpp"


void
idk::Transform_CS::stage_A( idk::EngineAPI &api )
{

}


void
idk::Transform_CS::onObjectAssignment( int obj_id, idk::Engine &engine )
{
    m_keys[obj_id] = m_matrices.create(glm::mat4(1.0f));

    m_parent[0]      = -1;
    m_parent[obj_id] = -1;
    giveChild(0, obj_id);

}


void
idk::Transform_CS::onObjectCreation( int obj_id, idk::Engine &engine )
{
    engine.giveComponent(obj_id, this->ID());
}


void
idk::Transform_CS::onObjectDeletion( int obj_id, idk::Engine &engine )
{
    auto &children = getChildren(obj_id);
    int  parent_id = m_parent[obj_id];

    for (int child_id: children)
    {
        if (parent_id != -1)
        {
            giveChild(parent_id, child_id);
        }

        m_children[obj_id].erase(child_id);
    }


    int &key = m_keys[obj_id];
    m_matrices.destroy(key);
    key = -1;
}


void
idk::Transform_CS::onObjectCopy( int src_obj_id, int dest_obj_id, idk::Engine &engine )
{
    engine.giveComponent(dest_obj_id, this->m_id);
}


void 
idk::Transform_CS::translate( int obj_id, glm::vec3 v )
{
    #ifdef IDK_DEBUG
        if (m_keys[obj_id] == -1)
        {
            std::cout << "m_transform_IDs[" << obj_id << "] == -1" << std::endl;
            exit(1);
        }
    #endif

    glm::mat4 &mat = getModelMatrix(obj_id);
    mat = glm::translate(mat, v);
    // getTransform(obj_id).translate(v);
}


void
idk::Transform_CS::rotateX( int obj_id, float r )
{
    glm::mat4 &mat = getModelMatrix(obj_id);
    mat = glm::rotate(mat, r, glm::vec3(1.0f, 0.0f, 0.0f));
}


void
idk::Transform_CS::rotateY( int obj_id, float r )
{
    glm::mat4 &mat = getModelMatrix(obj_id);
    mat = glm::rotate(mat, r, glm::vec3(0.0f, 1.0f, 0.0f));
}


void
idk::Transform_CS::rotateZ( int obj_id, float r )
{
    glm::mat4 &mat = getModelMatrix(obj_id);
    mat = glm::rotate(mat, r, glm::vec3(0.0f, 0.0f, 1.0f));
}


void 
idk::Transform_CS::scale( int obj_id, glm::vec3 s )
{
    glm::mat4 &mat = getModelMatrix(obj_id);
    mat = glm::scale(mat, glm::vec3(s));
}


glm::vec3 &
idk::Transform_CS::getPosition( int obj_id )
{
    glm::mat4 &transform = getModelMatrix(obj_id);
    return *reinterpret_cast<glm::vec3 *>(&transform[3]);
}



void
idk::Transform_CS::giveChild( int parent_id, int child_id )
{
    // if (m_parent[parent_id] == child_id)
    // {
    //     return;
    // }

    int last_parent_id = m_parent[child_id];
    if (last_parent_id != -1)
    {
        m_children[last_parent_id].erase(child_id);
    }

    m_children[parent_id].insert(child_id);
    m_parent[child_id] = parent_id;
}


void
idk::Transform_CS::removeChild( int parent_id, int child_id )
{
    m_children[parent_id].erase(child_id);
    m_parent[child_id] = -1;
}


void
idk::Transform_CS::clearChildren( int parent_id )
{
    for (int child_id: m_children[parent_id])
    {
        m_parent[child_id] = -1;
    }

    m_children[parent_id].clear();
}


void
idk::Transform_CS::removeParent( int child_id )
{
    int current_parent = m_parent[child_id];
    if (current_parent != -1)
    {
        removeChild(current_parent, child_id);
    }

    m_parent[child_id] = -1;
}




glm::mat4 &
idk::Transform_CS::getModelMatrix( int obj_id )
{
    return m_matrices.get(m_keys[obj_id]);
}



glm::mat4
idk::Transform_CS::getParentModelMatrix( int obj_id )
{
    int parent = m_parent[obj_id];
    return (parent == -1) ? glm::mat4(1.0f) : getModelMatrix(parent);
}


glm::mat4
idk::Transform_CS::getModelMatrixParented( int obj_id )
{
    int key = m_keys[obj_id];
    glm::mat4 transform = getModelMatrix(key);

    int parent_id = m_parent[obj_id];

    while (parent_id != -1)
    {
        transform = getModelMatrix(m_keys[parent_id]) * transform;
        parent_id = m_parent[parent_id];
    }

    return transform;
}
