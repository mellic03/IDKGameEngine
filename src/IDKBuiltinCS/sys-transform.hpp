#pragma once

#include "idk_components.hpp"


namespace idk
{
    class TransformSys;

    struct TransformCmp
    {
        int        obj_id   = -1;
        glm::vec3  position = glm::vec3(0.0f);
        glm::vec3  delta    = glm::vec3(0.0f);
        glm::quat  rotation = glm::quat(glm::vec3(0.0f));
        float      pitch    = 0.0f;
        float      yaw      = 0.0f;
        float      roll     = 0.0f;
        glm::vec3  up       = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3  front    = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3  right    = glm::vec3(1.0f, 0.0f, 0.0f);

        float      scale    = 1.0f;
        glm::vec3  scale3   = glm::vec3(1.0f);


        size_t  serialize( std::ofstream &stream ) const;
        size_t  deserialize( std::ifstream &stream );
        void    onObjectAssignment( idk::EngineAPI &api, int obj_id );
        void    onObjectDeassignment( idk::EngineAPI &api, int obj_id );
        void    onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj );
    };

    struct AnchorCmp
    {
        int   obj_id    = -1;

        std::vector<int>   anchor_ids = std::vector<int>(1, -1);
        std::vector<float> distances  = std::vector<float>(1, 1.0f);

        size_t  serialize( std::ofstream &stream ) const;
        size_t  deserialize( std::ifstream &stream );
        void    onObjectAssignment( idk::EngineAPI &api, int obj_id );
        void    onObjectDeassignment( idk::EngineAPI &api, int obj_id );
        void    onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj );
    };

    struct SmoothFollowCmp
    {
        int obj_id    = -1;
        int anchor_id = -1;
        float speed   = 1.0f;

        size_t  serialize( std::ofstream &stream ) const;
        size_t  deserialize( std::ifstream &stream );
        void    onObjectAssignment( idk::EngineAPI &api, int obj_id );
        void    onObjectDeassignment( idk::EngineAPI &api, int obj_id );
        void    onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj );
    };

};



class idk::TransformSys: public idk::ecs::System, public idk::LuaAPI
{
private:

    inline static std::unordered_map<int, glm::mat4> cached_world;
    inline static std::unordered_map<int, glm::mat4> cached_local;
    inline static std::unordered_map<int, glm::mat4> cached_model;

    static glm::mat4    _computeLocalMatrix( int obj_id, bool scale = true );
    static glm::mat4    _computeWorldMatrix( int obj_id );
    static glm::mat4    _computeModelMatrix( int obj_id );

    struct TransformCallback
    {
        int     subject;
        int     target;
        float   alpha;
        float   stop;
        std::function<void()> callback;
    };

    inline static std::unordered_map<int, TransformCallback> m_look_callbacks;
    inline static std::unordered_map<int, TransformCallback> m_move_callbacks;

    static void         _updateCallbacks();


public:
    virtual void        init   ( idk::EngineAPI & ) final;
    virtual void        update ( idk::EngineAPI & ) final;
    virtual void        exposeToLua( lua_State *LS ) final;

    
    static idk::TransformCmp &getData( int obj_id );

    static void         FABRIK( int objA, int objB, int objC, glm::vec3 end_pos,
                                float dAB, float dBC, const glm::vec3& );

    static void         FABRIK( const glm::vec3 &posA, glm::vec3 &posB, glm::vec3 &posC,
                                const glm::vec3 &target_posC, const glm::vec3 &pole_target,
                                float distAB, float distBC );

    static void         rotateX( int obj_id, float f );
    static void         rotateY( int obj_id, float f );
    static void         rotateZ( int obj_id, float f );

    static void         pitch ( int obj_id, float f );
    static void         yaw   ( int obj_id, float f );
    static void         roll  ( int obj_id, float f );

    static float        lookTowards( int subject, int target, float alpha );
    static float        lookTowards( int subject, const glm::vec3&, float alpha );
    static float        moveTowards( int subject, int target, float alpha );
    static float        moveTowards( int subject, const glm::vec3&, float alpha );

    static void         lookTowardsCallback( int subject, int target, float alpha, float stop, std::function<void()> );
    static void         moveTowardsCallback( int subject, int target, float alpha, float stop, std::function<void()> );


    static void         recomputeTransformMatrices( int obj_id );

    static void         translateWorldspace( int obj_id, const glm::vec3 & );
    static void         translateLocalspace( int obj_id, const glm::vec3 & );

    static glm::vec3    getPositionWorldspace( int obj_id );
    static glm::vec3    getPositionLocalspace( int obj_id );
    static void         setPositionWorldspace( int obj_id, const glm::vec3 & );
    static void         setPositionLocalspace( int obj_id, const glm::vec3 & );

    static glm::vec3    getUp            ( int obj_id );
    static glm::vec3    getRight         ( int obj_id );
    static glm::vec3    getFront         ( int obj_id );

    static void         setSurfaceUp     ( int obj_id, const glm::vec3 &, float alpha );
    static glm::vec3    getSurfaceUp     ( int obj_id );
    static glm::vec3    getSurfaceRight  ( int obj_id );
    static glm::vec3    getSurfaceFront  ( int obj_id );

    static glm::mat4    getLocalMatrix   ( int obj_id, bool scale = true );
    static glm::mat4    getWorldMatrix   ( int obj_id );
    static glm::mat4    getModelMatrix   ( int obj_id );

    static float        getDistanceWorldspace( int a, int b );

    static bool         isInsideBoundingRect( int obj_id, const glm::vec3 & );


    // Convenience functions
    // -----------------------------------------------------------------------------------------
    static void moveUp    ( int obj_id, float f );
    static void moveRight ( int obj_id, float f );
    static void moveFront ( int obj_id, float f );

    static void moveSurfaceUp    ( int obj_id, float f );
    static void moveSurfaceRight ( int obj_id, float f );
    static void moveSurfaceFront ( int obj_id, float f );
    // -----------------------------------------------------------------------------------------
};


