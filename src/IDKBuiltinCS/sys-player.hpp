#pragma once

#include "idk_components.hpp"
#include "idk_systems.hpp"

#include <IDKECS/IDKECS.hpp>



namespace idk
{
    struct PlayerControllerCmp;
    struct ArmCmp;

    class  PlayerSys;
}




class idk::PlayerSys: public idk::ecs::System
{
private:
    static void     _keyInput( idk::EngineAPI &api, int player );
    static void     _mouseInput( idk::EngineAPI &api, int player );

public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

};


struct idk::PlayerControllerCmp
{
    int obj_id    = -1;
    int hinge_obj = -1;
    int cam_obj   = -1;
    int model_obj = -1;

    float walk_speed = 1.0f;
    float run_speed  = 1.0f;
    float jump_force = 1.0f;

    void    update( idk::EngineAPI& );
    void    input( idk::EngineAPI& );

    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    void    onObjectAssignment( idk::EngineAPI &api, int obj_id );
    void    onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    void    onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj );
};


static constexpr int STATE_MOVING_TO_TARGET  = 0;
static constexpr int STATE_ARRIVED_AT_TARGET = 1;
static constexpr int STATE_WAITING           = 2;

struct idk::ArmCmp
{
    int  obj_id      = -1;
    int  partner_id  = -1;
    int  rest_target = -1;
    int  ray_target  = -1;
    int  pole_target = -1;

    float m_alpha  = 0.0f;
    bool  moving   = false;
    bool  is_left  = true;
    bool  grounded = true;

    
    glm::vec3 prev_anchor_pos = glm::vec3(0.0f);
    glm::vec3 next_anchor_pos = glm::vec3(0.0f);
    glm::vec3 anchor_pos      = glm::vec3(0.0f);

    float distAB = 0.3f;
    float distBC = 0.45f;
    float stride = 1.0f;
    float step_speed  = 4.0f;
    float step_height = 0.25f;

    glm::vec3 elbow_pos  = glm::vec3(0.0f);
    glm::vec3 hand_pos   = glm::vec3(0.0f);
    glm::vec3 target_pos = glm::vec3(0.0f);

    void    update( idk::EngineAPI& );
    void    state_arrived( idk::EngineAPI& );
    void    state_moving( idk::EngineAPI& );
    void    draw( idk::EngineAPI& );


    size_t  serialize( std::ofstream &stream ) const;
    size_t  deserialize( std::ifstream &stream );
    void    onObjectAssignment( idk::EngineAPI &api, int obj_id );
    void    onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    void    onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj );
};


