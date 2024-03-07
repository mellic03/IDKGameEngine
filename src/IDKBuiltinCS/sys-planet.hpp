#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>


namespace idk
{
    class PlanetSys;

    struct SunCmp;
    struct PlanetCmp;
    struct AtmosphereCmp;

    struct LocalGravityCmp;
    struct PlanetActorCmp;

};



class idk::PlanetSys: public idk::ecs::System, public idk::LuaAPI
{
private:
    static void update_sun ( idk::EngineAPI & );
    static void update_planet( idk::EngineAPI & );
    static void update_atmosphere( idk::EngineAPI & );

    static idk::SunCmp          &_getSunCmp( int obj_id );
    static idk::AtmosphereCmp   &_getAtmosphereCmp( int obj_id );
    static idk::PlanetCmp       &_getPlanetCmp( int obj_id );
    static idk::PlanetActorCmp  &_getPlanetActorCmp( int obj_id );


public:
    virtual void  init   ( idk::EngineAPI & ) final {  };
    virtual void  update ( idk::EngineAPI & ) final;
    virtual void  exposeToLua ( lua_State * ) final;

    static int          nearestPlanet( const glm::vec3 &position );
    static glm::vec3    averageGravity( const glm::vec3 &position );
    static bool         actorHasPlanet( int obj_id );

    static int          isInsideLocalGravity( const glm::vec3 &pos, glm::vec3 &G );
    static int          isInsideLocalGravity( int obj_id, glm::vec3 &G );

    static float        getPlanetActorGravity( int obj_id );
    static void         disablePlanetActor( int obj_id );
    static void         enablePlanetActor( int obj_id );

};




struct idk::SunCmp
{
    #define IDK_STRUCT_MEMBERS(X) \
        X( int, obj_id,   -1 )

    IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
    #undef IDK_STRUCT_MEMBERS

    void onObjectAssignment( idk::EngineAPI &api, int obj_id )
    {
        this->obj_id = obj_id;
    };

    void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
    void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

};


struct idk::PlanetCmp
{
    #define IDK_STRUCT_MEMBERS(X) \
        X( int,       obj_id,   -1 ) \
        X( float,     gravity,   1.0f ) \
        X( float,     SOI,       128.0f) \
        X( glm::vec4, orbital_origin,    glm::vec4(0.0f)) \
        X( glm::vec4, orbital_radii,     glm::vec4(0.0f)) \
        X( glm::vec4, orbital_speed,     glm::vec4(0.0f)) \
        X( glm::vec4, orbital_increment, glm::vec4(0.0f))

    IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
    #undef IDK_STRUCT_MEMBERS

    void onObjectAssignment( idk::EngineAPI &api, int obj_id )
    {
        this->obj_id = obj_id;
    };

    void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
    void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj ) {  };

};


struct idk::AtmosphereCmp
{
    #define IDK_STRUCT_MEMBERS(X) \
        X( int,       obj_id,           -1 ) \
        X( int,       atmosphere_id,    -1 ) \
        X( float,     sealevel,         20.0f ) \
        X( glm::vec4, position,         glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)) \
        X( glm::vec4, wavelengths,      glm::vec4(700.0f, 530.0f, 440.0f, 0.0f) ) \
        X( float,     radius,           100.0f ) \
        X( float,     density_falloff,  1.01f ) \
        X( float,     scatter_strength, 2.00f ) \
        X( float,     atmosphere_scale, 1.25f )

    IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
    #undef IDK_STRUCT_MEMBERS

    void onObjectAssignment( idk::EngineAPI &api, int obj_id )
    {
        this->obj_id = obj_id;
        atmosphere_id = api.getRenderer().createAtmosphere();
    };

    void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
    {
        api.getRenderer().destroyAtmosphere(atmosphere_id);
        atmosphere_id = -1;
    };

    void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
    {
        std::cout << "ATMOSPHERE COPY!!!!!\n";
    };
};




struct idk::LocalGravityCmp
{
    int  obj_id     = -1;
    bool enabled    = true;

    size_t serialize( std::ofstream &stream ) const
    {
        size_t n = 0;
        n += idk::streamwrite(stream, obj_id);
        n += idk::streamwrite(stream, enabled);
        return n;
    }

    size_t deserialize( std::ifstream &stream )
    {
        size_t n = 0;
        n += idk::streamread(stream, obj_id);
        n += idk::streamread(stream, enabled);
        return n;
    }

    void onObjectAssignment( idk::EngineAPI &api, int obj_id )
    {
        this->obj_id = obj_id;
    }

    void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
    {

    }

    void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj )
    {

    }

};


struct idk::PlanetActorCmp
{
    int  obj_id     = -1;
    bool enabled    = true;
    bool has_planet = false;

    size_t serialize( std::ofstream & ) const;
    size_t deserialize( std::ifstream & );

    void onObjectAssignment( idk::EngineAPI &api, int obj_id );
    void onObjectDeassignment( idk::EngineAPI &api, int obj_id );
    void onObjectCopy( idk::EngineAPI &api, int src_obj, int dst_obj );

};
