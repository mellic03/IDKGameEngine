#pragma once

#include "idk_components.hpp"
#include "sys-transform.hpp"


namespace idk
{
    struct AudioEmitterCmp;
    struct AudioListenerCmp;
    class AudioSys;
};


class idk::AudioSys: public idk::ECS::System
{
    // inline static std::unordered_map<int, std::function<void()>> m_callbacks;

public:
    virtual void        init   ( idk::EngineAPI & ) final;
    virtual void        update ( idk::EngineAPI & ) final;

    static int          loadSound   ( const std::string &filepath );
    static void         assignSound ( int obj_id, int sound );
    static void         assignSound ( int obj_id, const std::string &filepath );

    // static void         playSoundCallback ( int obj_id, std::function<void()>);
    static void         playSound         ( int obj_id, bool loop );
    static void         stopSound         ( int obj_id );
    static void         resumeSound       ( int obj_id );
    static void         pauseSound        ( int obj_id );
    static void         setVolume         ( int obj_id, float volume );


    static bool         isFinished( int obj_id );
    static bool         isPlaying( int obj_id );

};


struct idk::AudioEmitterCmp
{
    int         obj_id     = -1;
    int         emitter_id = -1;
    float       volume     = -1;
    std::string filepath   = "";

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );

};

struct idk::AudioListenerCmp
{
    #define IDK_STRUCT_MEMBERS(X) \
        X( int, obj_id, -1 ) \

    IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
    #undef IDK_STRUCT_MEMBERS

    static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
    {
        // this->obj_id = obj_id;
    };

    static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id )
    {

    };

    static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj ) {  };

};
