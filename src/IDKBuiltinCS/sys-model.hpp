#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>


namespace idk
{
    struct StaticHeightmapCmp;
    class  ModelSys;
}




class idk::ModelSys: public idk::ECS2::System
{
private:
    inline static   int m_heightmap_program = -1;
    inline static   int m_heightmap_RQ = -1;
    inline static   int m_shadow_RQ = -1;


public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    static void     assignModel( int obj_id, const std::string &filepath );
    static void     assignShader_gpass( int obj_id, const std::string &shader_name );

};



struct idk::StaticHeightmapCmp
{
    int obj_id = -1;
    int model  = -1;
    idk::TextureWrapper      heightmap;
    std::vector<std::string> textures;
    std::vector<uint32_t>    texture_ids;

    size_t  serialize            ( std::ofstream &stream ) const;
    size_t  deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, int obj_id );
    static void onObjectCopy         ( int src_obj, int dst_obj );
};


