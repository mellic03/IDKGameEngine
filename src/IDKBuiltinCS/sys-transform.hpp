#pragma once

#include <libidk/idk_transform.hpp> 
#include "idk_components.hpp"



namespace idk
{
    class TransformSys;

    struct TransformCmp;

    struct IKCmp
    {
        int obj_id       = -1;
        int pole_target  = -1;
        int chain_length = 3;
        std::vector<float> distances = { 0.5f, 0.6f };

        size_t  serialize( std::ofstream &stream ) const;
        size_t  deserialize( std::ifstream &stream );
        static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );
    };


    struct LookTowardCmp
    {
        int obj_id    = -1;
        int target_id = -1;

        size_t  serialize( std::ofstream &stream ) const;
        size_t  deserialize( std::ifstream &stream );
        static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );
    };


    struct AnchorCmp
    {
        int   obj_id    = -1;

        std::vector<int>   anchor_ids = std::vector<int>(1, -1);
        std::vector<float> distances  = std::vector<float>(1, 1.0f);

        size_t  serialize( std::ofstream &stream ) const;
        size_t  deserialize( std::ifstream &stream );
        static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );
    };


    struct SmoothFollowCmp
    {
        int obj_id    = -1;
        int anchor_id = -1;
        float speed   = 1.0f;

        size_t  serialize( std::ofstream &stream ) const;
        size_t  deserialize( std::ifstream &stream );
        static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );
    };


    struct RotateCmp
    {
        int       obj_id    = -1;
        float     magnitude = 0.0f;
        glm::vec3 axis      = glm::vec3(0.0f, 1.0f, 0.0f);

        size_t  serialize( std::ofstream &stream ) const;
        size_t  deserialize( std::ifstream &stream );
        static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
        static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );
    };

};



class idk::TransformSys: public idk::ECS::System
{
private:

    glm::mat4    _computeLocalMatrix( int obj_id, bool scale = true );
    glm::mat4    _computeWorldMatrix( int obj_id );
    glm::mat4    _computeModelMatrix( int obj_id );
    void         _computeTransform( int obj_id, const glm::mat4 &parent );

    struct TransformCallback
    {
        int     subject;
        int     target;
        float   alpha;
        float   stop;
        std::function<void()> callback;
    };

    std::unordered_map<int, TransformCallback> m_look_callbacks;
    std::unordered_map<int, TransformCallback> m_move_callbacks;

    void         _updateCallbacks();


public:
    virtual void        init   ( idk::EngineAPI & ) final;
    virtual void        update ( idk::EngineAPI & ) final;

    
    void         FABRIK( int objA, int objB, int objC, glm::vec3 end_pos,
                                float dAB, float dBC, const glm::vec3& );

    void         FABRIK( const glm::vec3 &posA, glm::vec3 &posB, glm::vec3 &posC,
                                const glm::vec3 &pole_target,
                                float distAB, float distBC );

    void         FABRIK( std::vector<glm::vec3> &positions,
                                const std::vector<float> &distances,
                                const glm::vec3 &pole_target );

    void         FABRIK( std::vector<glm::vec3> &positions,
                                const std::vector<float> &distances );

    void         FABRIK( int chain_length, int end_obj,
                                const std::vector<float> &distances,
                                const glm::vec3 &pole_target );

    void         pitch ( int obj_id, float f );
    void         yaw   ( int obj_id, float f );
    void         roll  ( int obj_id, float f );

    float        lookTowards( int subject, int target, float alpha );
    float        lookTowards( int subject, const glm::vec3&, float alpha );
    float        moveTowards( int subject, int target, float alpha );
    float        moveTowards( int subject, const glm::vec3&, float alpha );

    void         lookTowardsCallback( int subject, int target, float alpha, float stop, std::function<void()> );
    void         moveTowardsCallback( int subject, int target, float alpha, float stop, std::function<void()> );


    void         recomputeTransformMatrices( int obj_id );

    void         translateWorldspace( int obj_id, const glm::vec3& );
    void         translateLocalspace( int obj_id, const glm::vec3& );

    void         rotateLocalAxis( int obj_id, const glm::vec3&, float );
    void         rotateWorldAxis( int obj_id, const glm::vec3&, float );


    TransformCmp &getTransformCmp( int obj_id );
    Transform    &getTransform   ( int obj_id );

    glm::vec3    getPositionWorldspace( int obj_id );

    glm::vec3   &getLocalPosition( int obj_id );
    glm::vec3    getLocalPositionDelta( int obj_id );
    glm::quat   &getLocalRotation( int obj_id );
    glm::vec4   &getScale( int obj_id );
    glm::vec3   &getXYZScale( int obj_id );
    float       &getUniformScale( int obj_id );

    glm::vec3    getWorldPosition( int obj_id );
    glm::vec3    getWorldPositionDelta( int obj_id );
    glm::quat    getWorldRotation( int obj_id );

    void         setWorldPosition( int obj_id, const glm::vec3 & );
    void         setPositionLocalspace( int obj_id, const glm::vec3 & );

    glm::vec3    getUp            ( int obj_id );
    glm::vec3    getRight         ( int obj_id );
    glm::vec3    getFront         ( int obj_id );
    void         setFront         ( int obj_id, const glm::vec3& );

    glm::mat4    getLocalMatrix   ( int obj_id, bool scale = true );
    glm::mat4    getWorldMatrix   ( int obj_id );
    glm::mat4    getModelMatrix   ( int obj_id );

    float        getDistanceWorldspace( int a, int b );

    bool         isInsideBoundingRect( int obj_id, const glm::vec3 & );


    // Convenience functions
    // -----------------------------------------------------------------------------------------
    void moveUp    ( int obj_id, float f );
    void moveRight ( int obj_id, float f );
    void moveFront ( int obj_id, float f );
    // -----------------------------------------------------------------------------------------
};




struct idk::TransformCmp
{
    int obj_id = -1;

    idk::Transform transform;

    float pitch = 0.0f;
    float yaw   = 0.0f;
    float roll  = 0.0f;

    // Runtime data, not saved to disk
    // -----------------------------------------------------------------------------------------
    bool       is_dirty = true;

    glm::mat4  world = glm::mat4(0.0f);
    glm::mat4  local = glm::mat4(0.0f);
    glm::mat4  model = glm::mat4(0.0f);

    glm::mat4  prev_world = glm::mat4(1.0f);
    glm::mat4  prev_local = glm::mat4(1.0f);
    glm::mat4  prev_model = glm::mat4(1.0f);

    glm::vec3  up       = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3  right    = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3  front    = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::quat  world_rotation = glm::quat(glm::vec3(0.0f));

    // -----------------------------------------------------------------------------------------

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );
};
