#include "sys-transform2.hpp"

#include <libidk/idk_log.hpp>
#include <libidk/idk_geometry.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>




void
idk::Transform2Sys::init( idk::EngineAPI &api )
{
    
}



void
idk::Transform2Sys::update( idk::EngineAPI &api )
{
    auto &ecs = getECS();
    float dtime = api.getEngine().deltaTime();

    for (auto &cmp: ecs.getComponentArray<idk::Transform2Cmp>())
    {
        if (ecs.hasParent(cmp.obj_id) == false)
        {
            _compute_transforms(cmp.obj_id, glm::mat4(1.0f));
        }
    }
}


void
idk::Transform2Sys::_compute_transforms( int obj_id, const glm::mat4 &parent )
{
    auto &ecs = getECS();
    auto &cmp = getCmp(obj_id);

    glm::mat4 T = glm::translate(glm::mat4(1.0f), cmp.position);
    glm::mat4 R = glm::mat4_cast(cmp.rotation);

    cmp.model = parent * T * R;
    cmp.parent = parent;

    for (int child_id: ecs.getChildren(obj_id))
    {
        _compute_transforms(child_id, cmp.model);
    }
}



idk::Transform2Cmp&
idk::Transform2Sys::getCmp( int obj_id )
{
    return getECS().getComponent<idk::Transform2Cmp>(obj_id);
}

glm::mat4&
idk::Transform2Sys::getModelMatrix( int obj_id )
{
    return getCmp(obj_id).model;
}

glm::mat4&
idk::Transform2Sys::getParentMatrix( int obj_id )
{
    return getCmp(obj_id).parent;
}





void
idk::Transform2Sys::moveUp( int obj_id, float f )
{
    auto &cmp = getCmp(obj_id);
    cmp.position += f * cmp.up;
}


void
idk::Transform2Sys::moveRight( int obj_id, float f )
{
    auto &cmp = getCmp(obj_id);
    cmp.position += f * cmp.right;
}


void
idk::Transform2Sys::moveFront( int obj_id, float f )
{
    auto &cmp = getCmp(obj_id);
    cmp.position += f * cmp.front;
}





size_t
idk::Transform2Cmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    return n;
};


size_t
idk::Transform2Cmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    return n;
};


void
idk::Transform2Cmp::onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{
    auto &cmp = ecs.getComponent<Transform2Cmp>(obj_id);
    cmp.up    = glm::vec3(0, 1, 0);
    cmp.right = glm::vec3(1, 0, 0);
    cmp.front = glm::vec3(0, 0, -1);

    cmp.model = glm::translate(glm::mat4(1.0f), cmp.position);
    cmp.parent = glm::mat4(1.0f);
};


void
idk::Transform2Cmp::onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
{

};


void
idk::Transform2Cmp::onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj )
{
    // auto &src = idk::ECS2::getComponent<Transform2Cmp>(src_obj);
    // auto &dst = idk::ECS2::getComponent<Transform2Cmp>(dst_obj);
    // dst.obj_id = dst_obj;
};
