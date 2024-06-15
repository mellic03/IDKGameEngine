#pragma once

#include <IDKECS/IDKECS.hpp>
#include <IDKAudio/IDKAudio.hpp>
#include <IDKGameEngine/IDKGameEngine.hpp>

#include "../../external/include/idk_icons/idk_Icons.hpp"
#include <libidk/idk_serialize.hpp>



namespace idk
{

    struct IconCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int, obj_id, -1 ) \
            X( std::string, icon, ICON_FA_CUBE )

        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            // this->obj_id = obj_id;
        };

        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        static void onObjectCopy( int src_obj, int dst_obj ) {  };

    };


    struct DirlightCmp
    {
        int obj_id   = -1;
        int light_id = -1;
        glm::vec4 diffuse = glm::vec4(1.0f);
        glm::vec4 ambient = glm::vec4(0.1f);

        size_t serialize( std::ofstream &stream ) const
        {
            size_t n = 0;
            n += idk::streamwrite(stream, obj_id);
            n += idk::streamwrite(stream, diffuse);
            n += idk::streamwrite(stream, ambient);
            return n;
        };

        size_t deserialize( std::ifstream &stream )
        {
            size_t n = 0;
            n += idk::streamread(stream, obj_id);
            n += idk::streamread(stream, diffuse);
            n += idk::streamread(stream, ambient);
        
            light_id = -1;

            return n;
        };

        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            auto &cmp = ECS2::getComponent<DirlightCmp>(obj_id);
            cmp.light_id = api.getRenderer().createDirlight();
        };

        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {
            
        };

        static void onObjectCopy( int src_obj, int dst_obj )
        {
            std::cout << "DirlightCmp::onObjectCopy\n";
        };

    };

    struct PointlightCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int,       obj_id,    -1 ) \
            X( int,       light_id,  -1 ) \
            X( glm::vec4, diffuse,   glm::vec4(1.0f) ) \
            X( float,     radius,    16.0f )
    
        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            auto &cmp = ECS2::getComponent<PointlightCmp>(obj_id);
            cmp.light_id = api.getRenderer().createPointlight();
        };

        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {
            auto &cmp = ECS2::getComponent<PointlightCmp>(obj_id);
            api.getRenderer().destroyPointlight(cmp.light_id);
        };

        static void onObjectCopy( int src_obj, int dst_obj ) {  };

    };

    struct SpotlightCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int,       obj_id,    -1 ) \
            X( int,       light_id,  -1 ) \
            X( glm::vec4, diffuse,   glm::vec4(1.0f) ) \
            X( glm::vec3, angle,     glm::vec3(0.8f, 0.7f, 0.0f) ) \
            X( float,     radius,    16.0f )
    
        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            auto &cmp = ECS2::getComponent<SpotlightCmp>(obj_id);
            cmp.light_id = api.getRenderer().createSpotlight();
        };
    
        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {
            auto &cmp = ECS2::getComponent<SpotlightCmp>(obj_id);
            api.getRenderer().destroyPointlight(cmp.light_id);
        };

        static void onObjectCopy( int src_obj, int dst_obj ) {  };

    };


    struct ModelCmp
    {
        int         obj_id      = -1;
        int         model_id    = -1;
        bool        visible     = true;
        bool        shadowcast  = true;
        bool        viewspace   = false;
        std::string filepath    = "";

        bool        shader_enabled = false;
        int         render_queue   = -1;
        std::string shader_name    = "";


        size_t serialize( std::ofstream &stream ) const
        {
            size_t n = 0;
            n += idk::streamwrite(stream, obj_id);
            n += idk::streamwrite(stream, model_id);
            n += idk::streamwrite(stream, visible);
            n += idk::streamwrite(stream, shadowcast);
            n += idk::streamwrite(stream, viewspace);
            n += idk::streamwrite(stream, filepath);
            n += idk::streamwrite(stream, shader_enabled);
            n += idk::streamwrite(stream, render_queue);
            n += idk::streamwrite(stream, shader_name);
            return n;
        };

        size_t deserialize( std::ifstream &stream )
        {
            size_t n = 0;
            n += idk::streamread(stream, obj_id);
            n += idk::streamread(stream, model_id);
            n += idk::streamread(stream, visible);
            n += idk::streamread(stream, shadowcast);
            n += idk::streamread(stream, viewspace);
            n += idk::streamread(stream, filepath);
            n += idk::streamread(stream, shader_enabled);
            n += idk::streamread(stream, render_queue);
            n += idk::streamread(stream, shader_name);

            model_id = -1;

            return n;
        };

        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            ModelCmp &cmp = idk::ECS2::getComponent<ModelCmp>(obj_id);

            if (cmp.model_id == -1 && cmp.filepath != "")
            {
                cmp.model_id = api.getRenderer().loadModel(cmp.filepath);
            }
        };

        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {

        };

        static void onObjectCopy( int src_obj, int dst_obj )
        {
            ModelCmp &src = idk::ECS2::getComponent<ModelCmp>(src_obj);
            ModelCmp &dst = idk::ECS2::getComponent<ModelCmp>(dst_obj);
            
            dst.model_id    = src.model_id;
            dst.visible     = src.visible;
            dst.shadowcast  = src.shadowcast;
            dst.viewspace   = src.viewspace;
            dst.filepath    = src.filepath;
        };

    };


    struct CameraCmp
    {
        int   obj_id   = -1;
        int   cam_id   = -1;
        float gamma    = 2.2f;
        float exposure = 1.0f;
        float bloom    = 0.0f;
        float fov      = 90.0f;

        glm::vec2 chromatic_r;
        glm::vec2 chromatic_g;
        glm::vec2 chromatic_b;
        glm::vec4 chromatic_strength;
    
        size_t serialize( std::ofstream &stream ) const
        {
            size_t n = 0;
            n += idk::streamwrite(stream, obj_id);
            n += idk::streamwrite(stream, cam_id);
            n += idk::streamwrite(stream, gamma);
            n += idk::streamwrite(stream, exposure);
            n += idk::streamwrite(stream, bloom);
            n += idk::streamwrite(stream, fov);
            n += idk::streamwrite(stream, chromatic_r);
            n += idk::streamwrite(stream, chromatic_g);
            n += idk::streamwrite(stream, chromatic_b);
            n += idk::streamwrite(stream, chromatic_strength);
            return n;
        };

        size_t deserialize( std::ifstream &stream )
        {
            size_t n = 0;
            n += idk::streamread(stream, obj_id);
            n += idk::streamread(stream, cam_id);
            n += idk::streamread(stream, gamma);
            n += idk::streamread(stream, exposure);
            n += idk::streamread(stream, bloom);
            n += idk::streamread(stream, fov);
            n += idk::streamread(stream, chromatic_r);
            n += idk::streamread(stream, chromatic_g);
            n += idk::streamread(stream, chromatic_b);
            n += idk::streamread(stream, chromatic_strength);
            return n;
        };


        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            // this->obj_id = obj_id;
        };

        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        static void onObjectCopy( int src_obj, int dst_obj ) {  };

    };


    struct AudioEmitterCmp
    {
        int         obj_id     = -1;
        int         emitter_id = -1;
        float       volume     = -1;
        std::string filepath   = "";
    
        size_t serialize( std::ofstream &stream ) const
        {
            size_t n = 0;
            n += idk::streamwrite(stream, obj_id);
            n += idk::streamwrite(stream, emitter_id);
            n += idk::streamwrite(stream, volume);

            std::string str = std::filesystem::relative(filepath);
            idk_printvalue(str);

            n += idk::streamwrite(stream, str);
            return n;
        };

        size_t deserialize( std::ifstream &stream )
        {
            size_t n = 0;
            n += idk::streamread(stream, obj_id);
            n += idk::streamread(stream, emitter_id);
            n += idk::streamread(stream, volume);
            n += idk::streamread(stream, filepath);
            return n;
        };

        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            // this->obj_id = obj_id;
        };
    
        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )
        {
        };

        static void onObjectCopy( int src_obj, int dst_obj )
        {

        };

    };


    struct AudioListenerCmp
    {
        #define IDK_STRUCT_MEMBERS(X) \
            X( int, obj_id, -1 ) \
    
        IDK_STRUCT_BODY(IDK_STRUCT_MEMBERS)
        #undef IDK_STRUCT_MEMBERS

        static void onObjectAssignment( idk::EngineAPI &api, int obj_id )
        {
            // this->obj_id = obj_id;
        };

        static void onObjectDeassignment( idk::EngineAPI &api, int obj_id )       {  };
        static void onObjectCopy( int src_obj, int dst_obj ) {  };

    };

};




