#include "sys-transform.hpp"

#include <libidk/idk_log.hpp>
#include <libidk/idk_geometry.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>


size_t
idk::TransformCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;

    // Transform t = Transform::fromGLM(local);
    // t.scale = glm::vec4(scale3, scale);

    // n += idk::streamwrite(stream, obj_id);
    // n += idk::streamwrite(stream, t);

    // n += idk::streamwrite(stream, pitch);
    // n += idk::streamwrite(stream, yaw);
    // n += idk::streamwrite(stream, roll);

    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, transform);
    n += idk::streamwrite(stream, pitch);
    n += idk::streamwrite(stream, yaw);
    n += idk::streamwrite(stream, roll);

    return n;
};


size_t
idk::TransformCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    // n += idk::streamread(stream, obj_id);
    // n += idk::streamread(stream, position);
    // n += idk::streamread(stream, rotation);
    // n += idk::streamread(stream, pitch);
    // n += idk::streamread(stream, yaw);
    // n += idk::streamread(stream, roll);
    // n += idk::streamread(stream, up);
    // n += idk::streamread(stream, front);
    // n += idk::streamread(stream, right);
    // n += idk::streamread(stream, scale);
    // n += idk::streamread(stream, scale3);

    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, transform);
    n += idk::streamread(stream, pitch);
    n += idk::streamread(stream, yaw);
    n += idk::streamread(stream, roll);

    return n;
};


void
idk::TransformCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    auto &cmp = ECS2::getComponent<TransformCmp>(obj_id);
    cmp.transform.position = glm::vec3(1.0f); // = idk::Transform::fromGLM(glm::mat4(1.0f));
    // this->obj_id = obj_id;
};


void
idk::TransformCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::TransformCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<TransformCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<TransformCmp>(dst_obj);
    dst.obj_id = dst_obj;
};












size_t
idk::IKCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, pole_target);
    n += idk::streamwrite(stream, chain_length);
    return n;
};


size_t
idk::IKCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, pole_target);
    n += idk::streamread(stream, chain_length);
    return n;
};


void
idk::IKCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = obj_id;
};


void
idk::IKCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::IKCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<TransformCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<TransformCmp>(dst_obj);
    dst.obj_id = dst_obj;
};














size_t
idk::LookTowardCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, target_id);
    return n;
};


size_t
idk::LookTowardCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, target_id);
    return n;
};


void
idk::LookTowardCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

};


void
idk::LookTowardCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{

};


void
idk::LookTowardCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<LookTowardCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<LookTowardCmp>(dst_obj);
    dst.obj_id = dst_obj;
};











size_t
idk::AnchorCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, anchor_ids);
    n += idk::streamwrite(stream, distances);
    return n;
};


size_t
idk::AnchorCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, anchor_ids);
    n += idk::streamread(stream, distances);
    return n;
};


void
idk::AnchorCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = obj_id;
};


void
idk::AnchorCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    // this->obj_id = -1;
};


void
idk::AnchorCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<AnchorCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<AnchorCmp>(dst_obj);
    dst.obj_id = dst_obj;
};








size_t
idk::SmoothFollowCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, anchor_id);
    n += idk::streamwrite(stream, speed);
    return n;
};


size_t
idk::SmoothFollowCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, anchor_id);
    n += idk::streamread(stream, speed);
    return n;
};


void
idk::SmoothFollowCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

};


void
idk::SmoothFollowCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    
};


void
idk::SmoothFollowCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<SmoothFollowCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<SmoothFollowCmp>(dst_obj);
    dst.obj_id = dst_obj;
};










size_t
idk::RotateCmp::serialize( std::ofstream &stream ) const
{
    size_t n = 0;
    n += idk::streamwrite(stream, obj_id);
    n += idk::streamwrite(stream, magnitude);
    n += idk::streamwrite(stream, axis);
    return n;
};


size_t
idk::RotateCmp::deserialize( std::ifstream &stream )
{
    size_t n = 0;
    n += idk::streamread(stream, obj_id);
    n += idk::streamread(stream, magnitude);
    n += idk::streamread(stream, axis);
    return n;
};


void
idk::RotateCmp::onObjectAssignment( idk::EngineAPI &api, int obj_id )
{

};


void
idk::RotateCmp::onObjectDeassignment( idk::EngineAPI &api, int obj_id )
{
    
};


void
idk::RotateCmp::onObjectCopy( int src_obj, int dst_obj )
{
    auto &src = idk::ECS2::getComponent<SmoothFollowCmp>(src_obj);
    auto &dst = idk::ECS2::getComponent<SmoothFollowCmp>(dst_obj);
    dst.obj_id = dst_obj;
};


