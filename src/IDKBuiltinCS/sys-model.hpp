#pragma once

#include "idk_components.hpp"
#include <IDKECS/IDKECS.hpp>

#include <IDKGraphics/terrain/terrain.hpp>



namespace idk
{
    struct StaticHeightmapCmp;
    struct TerrainCmp;

    struct ModelCmp;
    class  ModelSys;
}




class idk::ModelSys: public idk::ECS::System
{
private:
    inline static int m_heightmap_program = -1;
    inline static int m_heightmap_RQ = -1;
    inline static int m_shadow_RQ = -1;
    inline static int m_alpha_cutoff_RQ = -1;


public:
    virtual void    init   ( idk::EngineAPI & ) final;
    virtual void    update ( idk::EngineAPI & ) final;

    void     assignModel( int obj_id, const char *filepath );
    void     assignModelLOD( int obj_id, int level, const char *filepath );
    void     assignCustomRQ( int obj_id, int RQ );

    void     assignShader_gpass( int obj_id, const char *shader_name );

};



struct idk::ModelCmp
{
    int         obj_id       = -1;
    int         model_id     = -1;
    bool        visible      = true;
    bool        shadowcast   = true;
    bool        environment  = false;
    bool        alpha_cutoff = false;
    int         custom_RQ    = -1;
    bool        viewspace    = false;

    glm::mat4   prev_T = glm::mat4(1.0f);
    std::string filepath = "";


    bool        shader_enabled = false;
    int         render_queue   = -1;
    std::string shader_name    = "";


    size_t serialize( std::ofstream &stream ) const;
    size_t deserialize( std::ifstream &stream );
    static void onObjectAssignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy( idk::ECS &ecs, int src_obj, int dst_obj );

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
    static void onObjectAssignment   ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy         ( idk::ECS &ecs, int src_obj, int dst_obj );
};



struct idk::TerrainCmp
{
    int obj_id = -1;

    size_t  serialize            ( std::ofstream &stream ) const;
    size_t  deserialize          ( std::ifstream &stream );
    static void onObjectAssignment   ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectDeassignment ( idk::EngineAPI &api, idk::ECS &ecs, int obj_id );
    static void onObjectCopy         ( idk::ECS &ecs, int src_obj, int dst_obj );
};


