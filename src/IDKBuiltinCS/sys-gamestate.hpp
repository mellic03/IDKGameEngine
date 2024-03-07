#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>



namespace idk
{
    struct game_milestones
    {
        enum
        {
            NO_PROGRESS                       = 0,
            ARRIVED_AT_MEDICAL_OFFICE         = 1,
            DIAGNOSED_WITH_SDS                = 2,
            RETURNED_HOME_FROM_MEDICAL_OFFICE = 3
        };
    };

    struct ProgressionEventCmp;
    struct ProgressionStateCmp;

    class  GameStateSys;
}



class idk::GameStateSys: public idk::ecs::System, public idk::LuaAPI
{
private:
    static idk::ProgressionEventCmp &_getProgressionEvent( int obj_id );
    static idk::ProgressionStateCmp &_getProgressionState( int obj_id );


public:
    virtual void        init   ( idk::EngineAPI & ) final;
    virtual void        update ( idk::EngineAPI & ) final;
    virtual void        exposeToLua ( lua_State *LS ) final;

    static void         assignScript( int obj_id, int script );

};



struct idk::ProgressionEventCmp
{
    int      obj_id           = -1;
    int      script_id        = -1;
    int      state_id         = -1;
    uint32_t prereq_progress  =  0;


    size_t serialize( std::ofstream &stream ) const
    {
        size_t n = 0;
        n += idk::streamwrite(stream, obj_id);
        n += idk::streamwrite(stream, state_id);
        n += idk::streamwrite(stream, script_id);
        n += idk::streamwrite(stream, prereq_progress);
        return n;
    };

    size_t deserialize( std::ifstream &stream )
    {
        size_t n = 0;
        n += idk::streamread(stream, obj_id);
        n += idk::streamread(stream, state_id);
        n += idk::streamread(stream, script_id);
        n += idk::streamread(stream, prereq_progress);
        return n;
    };

    void onObjectAssignment( idk::EngineAPI &api, int obj_id )
    {
        
    };

    void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
    {
        
    };

    void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
    {
        
    };
};



struct idk::ProgressionStateCmp
{
    int      obj_id   = -1;
    uint32_t progress = 0;
    uint32_t states[16];


    size_t serialize( std::ofstream &stream ) const
    {
        return idk::streamwrite(stream, obj_id);
    };

    size_t deserialize( std::ifstream &stream )
    {
        return idk::streamread(stream, obj_id);
    };

    void onObjectAssignment( idk::EngineAPI &api, int obj_id )
    {
        std::fill_n(states, 16, 0);
    };

    void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
    {
        
    };

    void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
    {
        
    };
};

