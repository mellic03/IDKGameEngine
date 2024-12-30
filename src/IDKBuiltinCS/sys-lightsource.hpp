#pragma once

#include <IDKECS/IDKECS.hpp>
#include <libidk/idk_serialize.hpp>



namespace idk
{
    struct DirlightCmp;
    struct PointlightCmp;
    struct SpotlightCmp;
    class  LightSys;

};



class idk::LightSys: public idk::ECS::System
{
public:
    virtual void init   ( idk::EngineAPI & ) final;
    virtual void update ( idk::EngineAPI & ) final;

};




struct idk::DirlightCmp
{
    int obj_id   = -1;
    int light_id = -1;
    IDK_Dirlight light;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};


struct idk::PointlightCmp
{
    int obj_id   = -1;
    int light_id = -1;
    IDK_Pointlight light;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};


struct idk::SpotlightCmp
{
    int obj_id   = -1;
    int light_id = -1;
    IDK_Spotlight light;

    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy( int src_obj, int dst_obj );
};


